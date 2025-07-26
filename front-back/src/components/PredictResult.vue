<template>
  <el-card class="result-card" shadow="hover">
    <h2>预测结果</h2>
    <p>30分钟后水位：<b>{{ result.predicted_water_level_30min }} m</b></p>
    <p>60分钟后水位：<b>{{ result.predicted_water_level_60min }} m</b></p>
    <p>
      风险等级：
      <b :style="{ color: levelColor[result.risk_level] }">{{ result.risk_level }}</b>
    </p>
    <p>风险评分：<b>{{ result.risk_score }}</b></p>
    <p><b>提示：</b>{{ result.alert_message }}</p>
  </el-card>
</template>

<script setup>
const props = defineProps({
  result: Object
})

const levelColor = {
  红色: 'red',
  橙色: 'orange',
  黄色: 'gold',
  蓝色: 'blue'
}
</script>
