<template>
  <div class="system-status-card">
    <div class="status-header">
      <span class="status-icon">📡</span>
      <span class="status-title">系统状态</span>
    </div>
    <div class="status-list">
      <div>在线设备：<b>1</b></div>
      <div>离线设备：<b>0</b></div>
      <div>异常设备：<b>0</b></div>
    </div>
  </div>
</template>

<script setup>

</script>

<style scoped>
.system-status-card {
  background: linear-gradient(90deg, #223a4d 60%, #1a2a3a 100%);
  border: 2px solid #00faff;
  border-radius: 12px;
  padding: 16px 24px;
  margin-bottom: 18px;
  display: flex;
  flex-direction: column;
  align-items: center;
  box-shadow: 0 2px 12px 0 rgba(0,80,160,0.12);
  font-size: 16px;
  min-width: 180px;
  animation: fadeIn 0.8s;
}
.status-header {
  display: flex;
  align-items: center;
  font-size: 20px;
  font-weight: bold;
  color: #00faff;
  margin-bottom: 8px;
}
.status-icon {
  font-size: 28px;
  margin-right: 10px;
}
.status-title {
  font-size: 20px;
}
.status-list {
  color: #fff;
  font-size: 16px;
  margin-top: 4px;
  text-align: center;
}
.status-list b {
  color: #00faff;
}
@keyframes fadeIn { from { opacity: 0; transform: translateY(-20px); } to { opacity: 1; transform: none; } }
</style> 