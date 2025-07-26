<template>
  <div>
    <h1>智慧水位预测系统</h1>
    <el-form :model="form" label-width="120px">
      <el-form-item label="当前水位 (m)">
        <el-input-number v-model="form.water_level" :min="0" :step="0.01" />
      </el-form-item>
      <el-form-item label="降雨量 (mm)">
        <el-input-number v-model="form.rainfall" :min="0" :step="0.1" />
      </el-form-item>
      <el-form-item label="温度 (°C)">
        <el-input-number v-model="form.temperature" :min="-30" :max="50" :step="0.1" />
      </el-form-item>
      <el-form-item label="气压 (hPa)">
        <el-input-number v-model="form.pressure" :min="900" :max="1100" :step="0.1" />
      </el-form-item>
      <el-form-item label="湿度 (%)">
        <el-input-number v-model="form.humidity" :min="0" :max="100" :step="1" />
      </el-form-item>
      <el-form-item>
        <el-button type="primary" @click="handleSubmit">提交预测</el-button>
      </el-form-item>
    </el-form>
  </div>
</template>

<script setup>
import { reactive } from 'vue'
import axios from 'axios'
import { ElMessage } from 'element-plus'
import { useRouter } from 'vue-router'

const router = useRouter()

const form = reactive({
  water_level: 3.5,
  rainfall: 10.0,
  temperature: 25.0,
  pressure: 1010.0,
  humidity: 80.0
})

async function handleSubmit() {
  try {
    const res = await axios.post('http://127.0.0.1:8000/api/predict', form)
    ElMessage.success('预测成功，正在跳转')
    router.push({
      path: '/result',
      query: {
        ...res.data,
        inputWater: form.water_level
      }
    })
  } catch (error) {
    ElMessage.error('预测失败，请检查后端是否运行')
    console.error(error)
  }
}
</script>
