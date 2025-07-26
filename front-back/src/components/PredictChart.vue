<template>
  <div ref="chartRef" style="width: 100%; height: 400px;"></div>
</template>

<script setup>
import * as echarts from 'echarts'
import { onMounted, ref, watch } from 'vue'

const props = defineProps({
  inputWater: Number,
  result: Object
})

const chartRef = ref()

function drawChart() {
  const chart = echarts.init(chartRef.value)
  chart.setOption({
    title: { text: '水位趋势预测图' },
    tooltip: {},
    xAxis: {
      type: 'category',
      data: ['当前', '30分钟后', '60分钟后']
    },
    yAxis: {
      type: 'value',
      name: '水位 (m)'
    },
    series: [{
      data: [
        props.inputWater,
        props.result.predicted_water_level_30min,
        props.result.predicted_water_level_60min
      ],
      type: 'line',
      smooth: true
    }]
  })
}

watch(() => props.result, () => {
  drawChart()
})

onMounted(() => {
  drawChart()
})
</script>
