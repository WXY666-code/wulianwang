<template>
  <div class="alarm-history-card">
    <div class="alarm-header">
      <span class="alarm-icon">🚨</span>
      <span class="alarm-title">告警历史</span>
    </div>
    <div class="alarm-list">
      <div v-for="(item, idx) in alarms" :key="idx" class="alarm-item">
        <span class="alarm-time">{{ item.time }}</span>
        <span class="alarm-msg">{{ item.msg }}</span>
      </div>
    </div>
  </div>
</template>

<script setup>
const alarms = [
  { time: '10:23', msg: '水位超限 5.3m' },
  { time: '09:58', msg: '风速超限 7.2m/s' },
  { time: '08:41', msg: '温度超限 29.1℃' }
]
</script>

<style scoped>
.alarm-history-card {
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
.alarm-header {
  display: flex;
  align-items: center;
  font-size: 20px;
  font-weight: bold;
  color: #ff5252;
  margin-bottom: 8px;
}
.alarm-icon {
  font-size: 28px;
  margin-right: 10px;
}
.alarm-title {
  font-size: 20px;
}
.alarm-list {
  color: #fff;
  font-size: 16px;
  margin-top: 4px;
  width: 100%;
}
.alarm-item {
  display: flex;
  align-items: center;
  margin-bottom: 4px;
}
.alarm-time {
  color: #ffd600;
  font-size: 14px;
  margin-right: 10px;
  min-width: 48px;
}
.alarm-msg {
  color: #fff;
  font-size: 15px;
}
@keyframes fadeIn { from { opacity: 0; transform: translateY(-20px); } to { opacity: 1; transform: none; } }
</style> 