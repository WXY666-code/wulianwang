<template>
  <div class="bottom-charts">
    <div class="top-cards-row">
      <DeviceStatusCard />
      <WeatherOverviewCard />
      <DeviceInfoCard />
      <AlarmHistoryCard />
      <SystemStatusCard />
    </div>
    <WarningModal v-if="showWarning" :info="warningInfo" @close="() => { showWarning = false; highlightMetric = '' }" />
    <div class="chart-row">
      <div v-for="metric in metrics" :key="metric.name" :class="['chart-item', { highlight: highlightMetric === metric.name }]">
        <div class="chart-title">{{ metric.name }}<span>({{ metric.unit }})</span></div>
        <div :ref="el => chartRefs[metric.name] = el" class="chart-box"></div>
      </div>
    </div>
  </div>
</template>

<script setup>
import { onMounted, ref, reactive, onBeforeUnmount } from 'vue'
import * as echarts from 'echarts'
import DeviceStatusCard from './DeviceStatusCard.vue'
import WarningModal from './WarningModal.vue'
import WeatherOverviewCard from './WeatherOverviewCard.vue'
import DeviceInfoCard from './DeviceInfoCard.vue'
import AlarmHistoryCard from './AlarmHistoryCard.vue'
import SystemStatusCard from './SystemStatusCard.vue'

const metrics = [
  { name: '风向', unit: '°', type: 'gauge', color: '#00faff' },
  { name: '温度', unit: '℃', type: 'line', color: '#ff9800' },
  { name: '湿度', unit: '%', type: 'line', color: '#2196f3' },
  { name: '风速', unit: 'm/s', type: 'line', color: '#00bfae' },
  { name: '阵风速度', unit: 'm/s', type: 'line', color: '#8e24aa' },
  { name: '累计降雨量', unit: 'mm', type: 'line', color: '#4caf50' },
  { name: '紫外线强度', unit: 'μW/cm²', type: 'line', color: '#e040fb' },
  { name: '光照强度', unit: 'lux', type: 'line', color: '#ffd600' },
  { name: '气压', unit: 'hPa', type: 'line', color: '#90caf9' },
  { name: '水位', unit: 'm', type: 'line', color: '#00faff' }
]

const chartRefs = reactive({})
const chartInstances = {}
const chartData = reactive({})
const maxPoints = 20
const lastValue = reactive({})

const showWarning = ref(false)
const warningInfo = ref({})
const highlightMetric = ref('')

const warningThresholds = {
  '水位': 5.2,
  '温度': 28,
  '风速': 7,
  // 可根据需要添加更多
}

function randomValue(metric) {
  // 真实感更强的平滑数据生成
  const ranges = {
    '风向': [0, 360, 10], // 变化幅度
    '温度': [15, 30, 0.3],
    '湿度': [40, 90, 1],
    '风速': [0, 8, 0.3],
    '阵风速度': [0, 12, 0.5],
    '累计降雨量': [0, 20, 0.2],
    '紫外线强度': [100, 400, 5],
    '光照强度': [10000, 30000, 300],
    '气压': [990, 1030, 0.5],
    '水位': [2.5, 5.5, 0.05]
  }
  const [min, max, step] = ranges[metric.name] || [0, 100, 1]
  if (lastValue[metric.name] === undefined) {
    // 初始值
    lastValue[metric.name] = (min + max) / 2
  }
  // 生成小幅度波动
  let next = lastValue[metric.name] + (Math.random() - 0.5) * step * 2
  next = Math.max(min, Math.min(max, next))
  lastValue[metric.name] = next
  // 保留小数位
  if (step < 1) return next.toFixed(2)
  if (step < 10) return next.toFixed(1)
  return Math.round(next)
}

function initCharts() {
  metrics.forEach(metric => {
    chartData[metric.name] = []
    if (metric.type === 'line') {
      for (let i = 0; i < maxPoints; i++) {
        chartData[metric.name].push(randomValue(metric))
      }
    } else {
      chartData[metric.name] = [randomValue(metric)]
    }
  })
}

function getRiskLevel(metricName, value) {
  // 简单分级：超阈值10%为“较高”，20%以上为“严重”，否则“一般”
  const threshold = warningThresholds[metricName]
  if (threshold === undefined) return '一般';
  const diff = value - threshold
  if (diff >= threshold * 0.2) return '严重';
  if (diff >= threshold * 0.1) return '较高';
  return '一般';
}

function checkWarnings() {
  metrics.forEach(metric => {
    if (metric.type === 'line' && warningThresholds[metric.name] !== undefined) {
      const val = Number(chartData[metric.name][chartData[metric.name].length - 1])
      if (val >= warningThresholds[metric.name]) {
        showWarning.value = true
        highlightMetric.value = metric.name
        warningInfo.value = {
          name: metric.name,
          value: val,
          unit: metric.unit,
          message: `${metric.name}已超阈值！当前值：${val}${metric.unit}`,
          riskLevel: getRiskLevel(metric.name, val)
        }
      }
    }
  })
}

function updateCharts() {
  metrics.forEach(metric => {
    if (metric.type === 'line') {
      chartData[metric.name].push(randomValue(metric))
      if (chartData[metric.name].length > maxPoints) chartData[metric.name].shift()
      chartInstances[metric.name]?.setOption({
        series: [{ data: chartData[metric.name], lineStyle: { color: metric.color }, areaStyle: { color: metric.color + '22' } }],
        xAxis: { data: Array.from({ length: chartData[metric.name].length }, (_, i) => i + 1) }
      })
    } else if (metric.type === 'gauge') {
      chartData[metric.name][0] = randomValue(metric)
      chartInstances[metric.name]?.setOption({
        series: [{ data: [{ value: chartData[metric.name][0] }], axisLine: { lineStyle: { color: [[1, metric.color]] } } }]
      })
    }
  })
  checkWarnings()
}

onMounted(() => {
  initCharts()
  metrics.forEach(metric => {
    if (!chartRefs[metric.name]) return
    let option
    if (metric.type === 'line') {
      option = {
        tooltip: { trigger: 'axis' },
        xAxis: {
          type: 'category',
          data: Array.from({ length: chartData[metric.name].length }, (_, i) => i + 1),
          axisLabel: { color: '#fff', fontSize: 12 }
        },
        yAxis: {
          type: 'value',
          name: metric.unit,
          nameTextStyle: { color: '#fff' },
          axisLabel: { color: '#fff', fontSize: 12 },
          ...(metric.name === '水位' ? { min: 2, max: 6, interval: 1 } : {})
        },
        series: [{
          data: chartData[metric.name],
          type: 'line',
          smooth: true,
          symbol: 'circle',
          lineStyle: { color: metric.color, width: 3 },
          areaStyle: { color: metric.color + '22' }
        }],
        grid: { left: 40, right: 20, top: 40, bottom: 30 }
      }
    } else if (metric.type === 'gauge') {
      option = {
        series: [{
          type: 'gauge',
          min: 0,
          max: 360,
          axisLine: { lineStyle: { width: 10, color: [[1, metric.color]] } },
          pointer: { width: 5 },
          detail: { valueAnimation: true, fontSize: 22, color: '#fff', formatter: '{value}°' },
          data: [{ value: chartData[metric.name][0] }],
          title: { color: '#fff', fontSize: 16 }
        }]
      }
    }
    chartInstances[metric.name] = echarts.init(chartRefs[metric.name])
    chartInstances[metric.name].setOption(option)
  })
  const timer = setInterval(updateCharts, 2000)
  window.addEventListener('resize', () => {
    Object.values(chartInstances).forEach(chart => chart?.resize())
  })
  onBeforeUnmount(() => {
    clearInterval(timer)
    Object.values(chartInstances).forEach(chart => chart?.dispose())
  })
})
</script>

<style scoped>
.bottom-charts {
  width: 100%;
  background: linear-gradient(135deg, #1a2a3a 60%, #223a4d 100%);
  border-radius: 12px;
  padding: 24px 0 0 0;
  margin-top: 16px;
  min-height: 400px;
  max-height: 520px;
  overflow-x: auto;
  box-shadow: 0 4px 24px 0 rgba(0,80,160,0.18);
}
.top-cards-row {
  display: flex;
  flex-wrap: nowrap;
  gap: 18px;
  margin-bottom: 10px;
  align-items: stretch;
  width: 100%;
  overflow-x: auto;
}
.top-cards-row > * {
  flex: 1 1 0;
  min-width: 180px;
  max-width: 340px;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
}
@media (max-width: 1100px) {
  .top-cards-row {
    gap: 10px;
  }
  .top-cards-row > * {
    min-width: 160px;
    font-size: 14px;
  }
}
.chart-row {
  display: flex;
  flex-wrap: nowrap;
  gap: 36px;
  justify-content: flex-start;
}
.chart-item {
  width: 220px;
  min-width: 180px;
  background: linear-gradient(135deg, #223a4d 60%, #1a2a3a 100%);
  border: 1.5px solid #00faff;
  border-radius: 16px;
  padding: 14px 10px 0 10px;
  margin-bottom: 14px;
  display: flex;
  flex-direction: column;
  align-items: center;
  box-shadow: 0 4px 24px 0 rgba(0,80,160,0.18);
  transition: box-shadow 0.2s;
}
.chart-item:hover {
  box-shadow: 0 8px 32px 0 rgba(0,200,255,0.25);
}
.chart-title {
  color: #00faff;
  font-size: 18px;
  font-weight: bold;
  margin-bottom: 8px;
  text-align: center;
  letter-spacing: 1px;
  text-shadow: 0 2px 8px #003366;
}
.chart-title span {
  font-size: 13px;
  color: #b2ebff;
  font-weight: normal;
  margin-left: 4px;
}
.chart-box {
  width: 170px;
  height: 120px;
}
.highlight {
  border: 2.5px solid #ff5252 !important;
  box-shadow: 0 0 18px 0 #ff5252cc, 0 4px 24px 0 rgba(0,80,160,0.18);
  animation: highlightPulse 1s infinite;
}
@keyframes highlightPulse {
  0% { box-shadow: 0 0 18px 0 #ff5252cc, 0 4px 24px 0 rgba(0,80,160,0.18); }
  50% { box-shadow: 0 0 32px 6px #ff5252ee, 0 4px 24px 0 rgba(0,80,160,0.18); }
  100% { box-shadow: 0 0 18px 0 #ff5252cc, 0 4px 24px 0 rgba(0,80,160,0.18); }
}
</style>
