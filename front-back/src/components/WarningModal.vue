<template>
  <transition name="warning-fade">
    <div class="warning-modal">
      <div class="warning-content">
        <div class="warning-icon">⚠️</div>
        <div class="warning-title">预警</div>
        <div class="warning-message">{{ info.message }}</div>
        <div v-if="info.riskLevel" class="warning-risk">风险等级：<b>{{ info.riskLevel }}</b></div>
        <div class="warning-detail">{{ info.name }} 当前值：<b>{{ info.value }}</b>{{ info.unit }}</div>
        <button class="close-btn" @click="$emit('close')">关闭</button>
      </div>
    </div>
  </transition>
</template>

<script setup>
const props = defineProps({
  info: { type: Object, required: true }
})
</script>

<style scoped>
.warning-modal {
  position: fixed;
  top: 0; left: 0; right: 0; bottom: 0;
  background: rgba(0,0,0,0.35);
  display: flex;
  align-items: center;
  justify-content: center;
  z-index: 9999;
}
.warning-content {
  background: #fffbe6;
  border: 2px solid #ffd600;
  border-radius: 16px;
  padding: 32px 38px 24px 38px;
  box-shadow: 0 8px 32px 0 rgba(255,200,0,0.18);
  display: flex;
  flex-direction: column;
  align-items: center;
  animation: shake 0.5s;
}
.warning-icon {
  font-size: 48px;
  margin-bottom: 10px;
  animation: blink 1s infinite;
}
.warning-title {
  font-size: 22px;
  color: #ff9800;
  font-weight: bold;
  margin-bottom: 8px;
}
.warning-message {
  font-size: 17px;
  color: #d84315;
  margin-bottom: 8px;
}
.warning-risk {
  font-size: 16px;
  color: #d32f2f;
  font-weight: bold;
  margin-bottom: 8px;
  letter-spacing: 1px;
  text-shadow: 0 2px 8px #ffd60044;
}
.warning-detail {
  font-size: 15px;
  color: #333;
  margin-bottom: 18px;
}
.close-btn {
  background: #ffd600;
  color: #333;
  border: none;
  border-radius: 8px;
  padding: 7px 22px;
  font-size: 15px;
  font-weight: bold;
  cursor: pointer;
  transition: background 0.2s;
}
.close-btn:hover {
  background: #ff9800;
  color: #fff;
}
@keyframes blink { 0%, 100% { opacity: 1; } 50% { opacity: 0.3; } }
@keyframes shake {
  0% { transform: translateX(0); }
  20% { transform: translateX(-8px); }
  40% { transform: translateX(8px); }
  60% { transform: translateX(-8px); }
  80% { transform: translateX(8px); }
  100% { transform: translateX(0); }
}
.warning-fade-enter-active, .warning-fade-leave-active {
  transition: opacity 0.3s;
}
.warning-fade-enter-from, .warning-fade-leave-to {
  opacity: 0;
}
</style> 