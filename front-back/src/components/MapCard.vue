<template>
  <div class="map-wrapper" ref="mapRef"></div>
</template>

<script setup>
import { onMounted, ref } from 'vue'
import * as echarts from 'echarts'

const mapRef = ref()

// 补全所有省份，未覆盖省份value为0
const allProvinces = [
  '北京', '天津', '上海', '重庆', '河北', '河南', '云南', '辽宁', '黑龙江', '湖南', '安徽', '山东', '新疆维吾尔自治区', '江苏', '浙江', '江西', '湖北', '广西壮族自治区', '甘肃', '山西', '内蒙古自治区', '陕西', '吉林', '福建', '贵州', '广东', '青海', '西藏自治区', '四川', '宁夏回族自治区', '海南', '台湾', '香港', '澳门'
]
const baseRisk = [
  { name: '北京', value: 30 },
  { name: '广东', value: 80 },
  { name: '四川', value: 50 },
  { name: '湖北', value: 60 },
  { name: '江苏', value: 45 }
]
const riskData = allProvinces.map(name => {
  const found = baseRisk.find(d => d.name === name)
  return { name, value: found ? found.value : 0 }
})

onMounted(async () => {
  const chart = echarts.init(mapRef.value)

  // 加载并注册地图
  const res = await fetch('/map/china.json')  // 确保路径和文件名小写
  const chinaMap = await res.json()
  echarts.registerMap('china', chinaMap)

  chart.setOption({
    title: {
      text: '南京天生桥景区 · 全国水利风险分布图',
      left: 'center',
      top: 10,
      textStyle: {
        color: '#00faff',
        fontSize: 24,
        fontWeight: 'bold',
        textShadowColor: '#003366',
        textShadowBlur: 8
      }
    },
    tooltip: {
      trigger: 'item',
      formatter: params => {
        if (params.name === '南京天生桥景区') {
          return `南京天生桥景区<br/>风险值：--<br/>水位：${(Math.random()*2+3).toFixed(2)} m`
        }
        return ''
      }
    },
    geo: {
      map: 'china',
      roam: true,
      label: { show: false },
      itemStyle: {
        areaColor: '#0a1a2f',
        borderColor: '#00faff',
        borderWidth: 2,
        shadowColor: '#00faff',
        shadowBlur: 16,
        opacity: 0.92
      },
      emphasis: {
        itemStyle: {
          areaColor: '#1e90ff',
          shadowColor: '#00faff',
          shadowBlur: 24,
          opacity: 1
        }
      },
      zlevel: 1
    },
    series: [
      {
        type: 'scatter',
        coordinateSystem: 'geo',
        symbol: 'circle',
        symbolSize: 0,
        data: [
          { name: '南京天生桥景区', value: [118.828, 31.596] }
        ],
        label: {
          show: true,
          formatter: [
            '{tag|南京天生桥景区}',
            '{arrow|▼}'
          ].join('\n'),
          rich: {
            tag: {
              color: '#fff',
              fontWeight: 'bold',
              fontSize: 18,
              backgroundColor: '#e53935',
              borderRadius: 8,
              padding: [6, 16],
              shadowColor: '#b71c1c',
              shadowBlur: 10,
              borderColor: '#fff',
              borderWidth: 2,
              align: 'center'
            },
            arrow: {
              color: '#e53935',
              fontSize: 28,
              height: 18,
              align: 'center',
              padding: [0, 0, -8, 0]
            }
          },
          position: 'top',
          z: 100
        },
        itemStyle: {
          color: 'transparent'
        },
        emphasis: {
          label: {
            backgroundColor: '#d32f2f',
            borderColor: '#fff',
            borderWidth: 2,
            color: '#fff',
            fontWeight: 'bold',
            fontSize: 22,
            shadowColor: '#b71c1c',
            shadowBlur: 16
          }
        },
        zlevel: 3
      }
    ]
  })

  window.addEventListener('resize', () => chart.resize())
})
</script>

<style scoped>
.map-wrapper {
  width: 100%;
  height: 100%;
  min-height: 500px;
  background: rgba(0, 30, 60, 0.2);
  border-radius: 8px;
}
</style>
