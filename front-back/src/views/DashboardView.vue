<template>
  <div class="dashboard-bg">
    <!-- 顶部标题栏 -->
    <div class="dashboard-header">
      <div class="title">🌊 南京天生桥景区 智慧水利可视化监测大屏</div>
      <div class="clock">{{ time }}</div>
    </div>

    <!-- 主体布局区域 -->
    <div class="dashboard-main">
      <div class="left-panel">
        <el-card class="info-card">
            <div class="label">[南京天生桥景区] 当前水位：</div>
            <div class="value">{{ data.inputWater }} m</div>
          </el-card>
          <el-card class="info-card">
            <div class="label">[南京天生桥景区] 30分钟后预测：</div>
            <div class="value">{{ data.predicted_water_level_30min }} m</div>
          </el-card>
          <el-card class="info-card">
            <div class="label">[南京天生桥景区] 60分钟后预测：</div>
            <div class="value">{{ data.predicted_water_level_60min }} m</div>
          </el-card>
          <el-card class="info-card">
            <div class="label">[南京天生桥景区] 风险等级：</div>
            <div class="value" :style="{ color: riskColors[data.risk_level] }">
              {{ data.risk_level }}
            </div>
          </el-card>
      </div>

      <!-- 中间地图 -->
      <div class="map-panel">
        <MapCard />
      </div>

      <!-- 右侧图表 -->
      <div class="right-panel">
        <div class="chart-title">南京天生桥景区 未来水位趋势预测</div>
        <div id="chart" class="chart-container"></div>
      </div>
    </div>

    <!-- 底部多指标图表区 -->
    <div class="dashboard-bottom">
      <BottomCharts />
    </div>
  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue'
import * as echarts from 'echarts'
import MapCard from '../components/MapCard.vue'
import BottomCharts from '../components/BottomCharts.vue'

// 实时时钟
const time = ref('')
function updateClock() {
  const now = new Date()
  time.value = now.toLocaleTimeString()
}
setInterval(updateClock, 1000)
updateClock()

// 模拟数据
const data = ref({
  inputWater: 3.45,
  predicted_water_level_30min: 4.12,
  predicted_water_level_60min: 5.02,
  risk_level: '橙色'
})

const riskColors = {
  红色: 'red',
  橙色: 'orange',
  黄色: '#ffcc00',
  蓝色: '#409EFF'
}

let chart = null
function fetchData() {
  const fake = {
    inputWater: (Math.random() * 2 + 3).toFixed(2),
    predicted_water_level_30min: (Math.random() * 2 + 4).toFixed(2),
    predicted_water_level_60min: (Math.random() * 2 + 5).toFixed(2),
    risk_level: ['蓝色', '黄色', '橙色', '红色'][Math.floor(Math.random() * 4)]
  }
  data.value = fake
  updateChart()
}

function updateChart() {
  if (!chart) return
  chart.setOption({
    series: [
      {
        data: [
          Number(data.value.inputWater),
          Number(data.value.predicted_water_level_30min),
          Number(data.value.predicted_water_level_60min)
        ]
      }
    ]
  })
}

onMounted(() => {
  chart = echarts.init(document.getElementById('chart'))
  chart.setOption({
    tooltip: { trigger: 'axis' },
    xAxis: {
      type: 'category',
      data: ['当前', '30分钟后', '60分钟后'],
      axisLabel: { color: '#fff' }
    },
    yAxis: {
      type: 'value',
      name: '水位 (m)',
      nameTextStyle: { color: '#fff' },
      axisLabel: { color: '#fff' }
    },
    series: [
      {
        type: 'line',
        smooth: true,
        symbol: 'circle',
        lineStyle: { color: '#00faff', width: 4 },
        areaStyle: { color: 'rgba(0,250,255,0.1)' },
        data: []
      }
    ]
  })

  fetchData()
  setInterval(fetchData, 10000)
})
</script>

<style scoped>
.dashboard-bg {
  display: flex;
  flex-direction: column;
  height: 100vh;
  min-height: 0;
  background: linear-gradient(to right bottom, #010f1c, #002f4b);
  color: #fff;
  font-family: 'Segoe UI', sans-serif;
  overflow: hidden;
}
.dashboard-header {
  flex: 0 0 70px;
  display: flex;
  justify-content: space-between;
  align-items: center;
  height: 70px;
  padding: 0 48px;
  font-size: 24px;
  background-color: #031627;
  border-bottom: 2px solid #00faff;
  border-radius: 0 0 18px 18px;
  box-shadow: 0 2px 12px 0 rgba(0,80,160,0.10);
}
.title {
  color: #00faff;
  font-weight: bold;
  text-shadow: 0 0 12px #00faff;
  font-size: 28px;
}
.clock {
  color: #ccc;
  font-size: 20px;
}
.dashboard-main {
  flex: 1 1 auto;
  min-height: 0;
  overflow: auto;
  display: flex;
  flex-direction: row;
  gap: 32px;
  padding: 32px 40px 0 40px;
  align-items: stretch;
}
.left-panel {
  width: 260px;
  display: flex;
  flex-direction: column;
  gap: 18px;
  align-items: center;
  justify-content: center;
}
.map-panel, .right-panel {
  min-height: 480px;
  height: 100%;
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;
}
.map-panel {
  flex: 2;
  background: rgba(0, 30, 60, 0.32);
  border: 2px solid #00faff;
  border-radius: 18px;
  box-shadow: 0 4px 24px 0 rgba(0,80,160,0.10);
  margin-bottom: 0;
  min-width: 0;
  overflow: hidden;
}
.right-panel {
  flex: 1;
  background: rgba(0, 30, 60, 0.32);
  border: 2px solid #00faff;
  border-radius: 18px;
  box-shadow: 0 4px 24px 0 rgba(0,80,160,0.10);
  padding: 18px 12px 0 12px;
  min-width: 0;
}
.chart-title {
  font-size: 20px;
  text-align: center;
  margin-bottom: 12px;
  color: #00faff;
  font-weight: bold;
}
.chart-container {
  width: 100%;
  height: 100%;
  min-height: 320px;
}
.info-card {
  background-color: rgba(0, 30, 60, 0.6);
  border: 1.5px solid #00faff;
  border-radius: 12px;
  text-align: center;
  font-size: 16px;
  box-shadow: 0 2px 12px 0 rgba(0,80,160,0.10);
  min-height: 90px;
  display: flex;
  flex-direction: column;
  justify-content: center;
}
.label {
  font-size: 16px;
  font-weight: bold;
  color: #00faff;
  line-height: 1.3;
  white-space: normal;
  word-break: break-all;
  text-align: center;
}
.value {
  font-size: 30px;
  margin-top: 12px;
  color: #fff;
  font-weight: bold;
  text-align: center;
}
.value[style*='orange'], .value[style*='#ffcc00'], .value[style*='red'] {
  color: #ffb300 !important;
  font-weight: bold;
}
.dashboard-bottom {
  flex: 0 0 280px;
  min-height: 280px;
  max-height: 340px;
  margin: 0 40px 18px 40px;
  border-radius: 18px;
  box-shadow: 0 4px 24px 0 rgba(0,80,160,0.10);
  border-top: 2px solid #00faff;
  background: none;
  padding: 0 10px 10px 10px;
  overflow-x: auto;
  overflow-y: visible;
  display: block;
}
</style>
