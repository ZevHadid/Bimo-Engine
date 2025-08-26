
function sleep(seconds)
    local start_time = os.time()
    while os.time() < start_time + seconds do
        -- Loop until the desired time has passed
    end
end
-- Rectangle in the center
rect = MakeShape('<rect x="300" y="250" width="200" height="100" fill="red"/>')
-- Circle in the center
circle = MakeShape('<circle cx="400" cy="300" r="50" fill="blue"/>')
-- Show shapes
rect:show()
circle:show()

-- Star polygon
star = MakeShape('<polygon points="400,100 420,140 460,140 430,170 440,210 400,190 360,210 370,170 340,140 380,140" fill="gold"/>')


star:show()

-- Additional shapes
green_rect = MakeShape('<rect x="100" y="100" width="80" height="80" fill="green"/>')
green_rect:show()

line = MakeShape('<line x1="50" y1="500" x2="750" y2="500" stroke="red"/>')
line:show()

print("All shapes created and shown!")

