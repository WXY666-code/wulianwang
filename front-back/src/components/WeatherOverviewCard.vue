<template>
  <div class="weather-overview-card">
    <div class="weather-header">
      <span class="weather-icon">☀️</span>
      <span class="weather-title">今日天气</span>
    </div>
    <div class="weather-info">
      <div>温度：<b>26℃</b></div>
      <div>湿度：<b>60%</b></div>
      <div>风速：<b>3.2m/s</b></div>
      <div>天气：<b>晴</b></div>
    </div>
  </div>
</template>

<script setup>
// 可后续接入真实天气API
</script>

<style scoped>
.weather-overview-card {
  background: linear-gradient(90deg, #223a4d 60%, #1a2a3a 100%);
  border: 2px solid #00faff;
  border-radius: 12px;
  padding: 16px 24px;
  margin-bottom: 18px;
  display: flex;
  flex-direction: column;
  align-items: flex-start;
  box-shadow: 0 2px 12px 0 rgba(0,80,160,0.12);
  font-size: 16px;
  min-width: 260px;
  max-width: 340px;
  animation: fadeIn 0.8s;
}
.weather-header {
  display: flex;
  align-items: center;
  font-size: 20px;
  font-weight: bold;
  color: #ffd600;
  margin-bottom: 8px;
}
.weather-icon {
  font-size: 28px;
  margin-right: 10px;
}
.weather-title {
  font-size: 20px;
}
.weather-info {
  color: #fff;
  font-size: 16px;
  margin-top: 4px;
}
.weather-info b {
  color: #ffd600;
}
@keyframes fadeIn { from { opacity: 0; transform: translateY(-20px); } to { opacity: 1; transform: none; } }
</style> 