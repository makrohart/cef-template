import { defineConfig } from 'vite'
import react from '@vitejs/plugin-react'

// https://vite.dev/config/
export default defineConfig({
  plugins: [react()],
  // 使用相对路径，便于 CEF 在 file:// 协议下正确加载 dist 资源
  base: './',
})
