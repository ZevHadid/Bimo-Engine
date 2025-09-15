import { defineConfig } from 'vite'
import react from '@vitejs/plugin-react'
import Components from 'unplugin-react-components/vite'

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [
    react(),
    Components({
      dirs: ['src/components'], // auto-import anything in this folder
      extensions: ['tsx'],      // only look for .tsx files
      deep: true,               // also check nested folders
      dts: 'src/components.d.ts' // generates types so TS knows <Button /> exists
    }),
  ],
})

