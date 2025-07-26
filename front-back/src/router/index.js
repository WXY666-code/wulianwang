import { createRouter, createWebHistory } from 'vue-router'
import PredictFormView from '../views/PredictFormView.vue'
import PredictResultView from '../views/PredictResultView.vue'
import DashboardView from '../views/DashboardView.vue'
const routes = [
  { path: '/', component: DashboardView },
  { path: '/dashboard', component: DashboardView },
  { path: '/form', component: PredictFormView },
  { path: '/result', component: PredictResultView }
]

const router = createRouter({
  history: createWebHistory(),
  routes
})

export default router
