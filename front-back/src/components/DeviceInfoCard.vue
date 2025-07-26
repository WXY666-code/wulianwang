<template>
  <div class="device-info-card">
    <div class="info-header">
      <span class="info-icon">🔧</span>
      <span class="info-title">设备信息</span>
    </div>
    <div class="info-list">
      <div>设备编号：<b>WX-2024-001</b></div>
      <div>安装位置：<b>南京天生桥景区</b></div>
      <div>电池电量：<b>87%</b></div>
      <div>运行时长：<b>12天</b></div>
    </div>
  </div>
</template>

<script setup>
// 可后续接入真实设备数据
</script>

<style scoped>
.device-info-card {
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
.info-header {
  display: flex;
  align-items: center;
  font-size: 20px;
  font-weight: bold;
  color: #00faff;
  margin-bottom: 8px;
}
.info-icon {
  font-size: 28px;
  margin-right: 10px;
}
.info-title {
  font-size: 20px;
}
.info-list {
  color: #fff;
  font-size: 16px;
  margin-top: 4px;
}
.info-list b {
  color: #00faff;
}
@keyframes fadeIn { from { opacity: 0; transform: translateY(-20px); } to { opacity: 1; transform: none; } }
</style> 