-- Rectangle
rect_id = create_shape('<rect x="50" y="50" width="200" height="100" fill="red"/>')
-- Circle
circle_id = create_shape('<circle cx="400" cy="300" r="50" fill="blue"/>')
-- Show shapes
show_shape(rect_id)
show_shape(circle_id)

-- David's star polygon
star_id = create_shape('<polygon points="400,100 420,140 460,140 430,170 440,210 400,190 360,210 370,170 340,140 380,140" fill="gold"/>')
show_shape(star_id)
