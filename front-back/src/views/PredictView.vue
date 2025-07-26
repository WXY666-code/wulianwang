<template>
  <div class="predict-view">
    <h1>智慧水位预测系统</h1>
    <PredictForm @onPredict="handlePredictResult" />
    <PredictResult v-if="result" :result="result" />
    <PredictChart v-if="result" :inputWater="inputWaterLevel" :result="result" />
  </div>
</template>

<script setup>
import { ref } from 'vue'
import PredictForm from '../components/PredictForm.vue'
import PredictResult from '../components/PredictResult.vue'
import PredictChart from '../components/PredictChart.vue'

const result = ref(null)
const inputWaterLevel = ref(null)

function handlePredictResult(data, inputWater) {
  result.value = data
  inputWaterLevel.value = inputWater
}
</script>
