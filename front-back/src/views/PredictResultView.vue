<template>
  <el-card class="result-card" shadow="hover">
    <h2>预测结果</h2>

    <el-descriptions title="预测详情" :column="1" border>
      <el-descriptions-item label="当前水位 (m)">
        {{ parseFloat($route.query.inputWater).toFixed(2) }}
      </el-descriptions-item>
      <el-descriptions-item label="30分钟后水位 (m)">
        {{ parseFloat($route.query.predicted_water_level_30min).toFixed(2) }}
      </el-descriptions-item>
      <el-descriptions-item label="60分钟后水位 (m)">
        {{ parseFloat($route.query.predicted_water_level_60min).toFixed(2) }}
      </el-descriptions-item>
      <el-descriptions-item label="风险等级">
        <b :style="{ color: levelColor[$route.query.risk_level] }">{{ $route.query.risk_level }}</b>
      </el-descriptions-item>
      <el-descriptions-item label="风险评分">
        {{ parseFloat($route.query.risk_score).toFixed(2) }}
      </el-descriptions-item>
      <el-descriptions-item label="提示">
        {{ $route.query.alert_message }}
      </el-descriptions-item>
    </el-descriptions>

    <div id="chart" style="width: 100%; height: 400px; margin-top: 30px;"></div>

    <div style="text-align: right; margin-top: 20px;">
      <el-button type="primary" @click="$router.push('/')">重新预测</el-button>
    </div>
  </el-card>
</template>

<script setup>
import { onMounted } from 'vue'
import * as echarts from 'echarts'

const levelColor = {
  红色: 'red',
  橙色: 'orange',
  黄色: 'gold',
  蓝色: 'blue'
}

onMounted(() => {
  const chart = echarts.init(document.getElementById('chart'))
  chart.setOption({
    title: { text: '未来水位变化趋势' },
    tooltip: {
      trigger: 'axis',
      formatter: '{b}：{c} m'
    },
    xAxis: {
      type: 'category',
      data: ['当前', '30分钟后', '60分钟后']
    },
    yAxis: {
      type: 'value',
      name: '水位 (m)',
      min: 'dataMin',
      max: 'dataMax'
    },
    series: [{
      data: [
        parseFloat(history.state?.inputWater || 0),
        parseFloat(history.state?.predicted_water_level_30min || 0),
        parseFloat(history.state?.predicted_water_level_60min || 0)
      ],
      type: 'line',
      smooth: true,
      symbol: 'circle',
      symbolSize: 10,
      areaStyle: {},
      lineStyle: {
        width: 3
      }
    }],
    color: ['#409EFF']
  })
})
</script>
