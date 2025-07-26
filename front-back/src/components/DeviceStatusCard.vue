<template>
  <div class="device-status-card" :class="statusClass">
    <div class="device-header">
      <span class="device-icon">🛰️</span>
      <span class="device-name">气象监测终端</span>
    </div>
    <div class="device-info">
      <div>状态：<span :class="statusClass">{{ statusText }}</span></div>
      <div>信号：<span :class="signalClass">{{ signalText }}</span></div>
      <div>最后通信：{{ lastCommTime }}</div>
    </div>
    <div v-if="status === 'connecting'" class="connecting-anim">正在连接服务器…</div>
  </div>
</template>

<script setup>
import { ref, computed, onMounted, onBeforeUnmount } from 'vue'
const status = ref('online') // online, offline, connecting
const signal = ref('strong') // strong, weak, lost
const lastCommTime = ref('刚刚')

const statusTextMap = { online: '在线', offline: '离线', connecting: '连接中' }
const signalTextMap = { strong: '强', weak: '弱', lost: '无' }

const statusText = computed(() => statusTextMap[status.value])
const signalText = computed(() => signalTextMap[signal.value])
const statusClass = computed(() => `status-${status.value}`)
const signalClass = computed(() => `signal-${signal.value}`)

function randomStatus() {
  const statusArr = ['online', 'offline', 'connecting']
  const signalArr = ['strong', 'weak', 'lost']
  status.value = statusArr[Math.floor(Math.random() * statusArr.length)]
  signal.value = signalArr[Math.floor(Math.random() * signalArr.length)]
  const now = new Date()
  lastCommTime.value = `${now.getHours()}:${now.getMinutes().toString().padStart(2, '0')}:${now.getSeconds().toString().padStart(2, '0')}`
}

onMounted(() => {
  setInterval(randomStatus, 10000)
})
</script>

<style scoped>
.device-status-card {
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
.device-header {
  display: flex;
  align-items: center;
  font-size: 20px;
  font-weight: bold;
  color: #00faff;
  margin-bottom: 8px;
}
.device-icon {
  font-size: 28px;
  margin-right: 10px;
}
.status-online { color: #00ff99; font-weight: bold; }
.status-offline { color: #ff5252; font-weight: bold; animation: blink 1s infinite; }
.status-connecting { color: #ffd600; font-weight: bold; animation: pulse 1.2s infinite; }
.signal-strong { color: #00e676; }
.signal-weak { color: #ffd600; }
.signal-lost { color: #ff5252; animation: blink 1s infinite; }
.connecting-anim { color: #ffd600; font-size: 15px; margin-top: 6px; animation: pulse 1.2s infinite; }
@keyframes blink { 0%, 100% { opacity: 1; } 50% { opacity: 0.3; } }
@keyframes pulse { 0% { opacity: 1; } 50% { opacity: 0.5; } 100% { opacity: 1; } }
@keyframes fadeIn { from { opacity: 0; transform: translateY(-20px); } to { opacity: 1; transform: none; } }
</style> 