import { useState } from 'react'
import reactLogo from './assets/react.svg'
import { Routes, Route, Link } from "react-router-dom"
import { open } from "@tauri-apps/api/dialog"
import viteLogo from '/vite.svg'
import './App.css'

import HomePage from "./pages/HomePage"

function App() {
  return (
    <div>
      {/* add shit here if you want idk */}

      <Routes>
        <Route path="/" element={<div className="page"><HomePage /></div>} />
      </Routes>
    </div>
  )
}
export default App
