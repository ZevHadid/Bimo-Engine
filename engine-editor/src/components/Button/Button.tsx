
import { useState } from "react"
import styles from "./Button.module.css"

interface ButtonProps {
  children: React.ReactNode
  onClick?: () => void
  type?: "button" | "submit" | "reset"
  style?: React.CSSProperties
}

export default function Button({ children, onClick, type = "button", style }: ButtonProps) {
  const [rippleStyle, setRippleStyle] = useState<React.CSSProperties | null>(null)

  const handleClick = (e: React.MouseEvent<HTMLButtonElement>) => {
    const rect = e.currentTarget.getBoundingClientRect()
    const size = Math.max(rect.width, rect.height)
    const x = e.clientX - rect.left - size / 2
    const y = e.clientY - rect.top - size / 2

    setRippleStyle({
      top: y + "px",
      left: x + "px",
      width: size + "px",
      height: size + "px",
    })

    setTimeout(() => setRippleStyle(null), 600)

    if (onClick) onClick()
  }

  return (
    <button
      type={type}
      className={styles.btn}
      onClick={handleClick}
      style={style}
    >
      {children}
      {rippleStyle && <span className={styles.ripple} style={rippleStyle} />}
    </button>
  )
}
