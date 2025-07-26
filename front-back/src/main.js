import { createApp } from 'vue'
import App from './App.vue'
import router from './router'
import ElementPlus from 'element-plus'
import 'element-plus/dist/index.css'

// ✅ 新增
import Particles from "vue3-particles"

const app = createApp(App)
app.use(router)
app.use(ElementPlus)
app.use(Particles) // ✅ 注册粒子插件
app.mount('#app')
