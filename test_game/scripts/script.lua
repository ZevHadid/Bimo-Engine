-- move_block.lua
game_title("Move Block with Arrow Keys")

-- Create a block
local block = MakeObject("../assets/images/green_block.jpg")
block:location(50, 50) -- Center of screen
block:scale(10)        -- 10% of window size
block:show()

local move_speed = 100          -- pixels per second
local block_x, block_y = 50, 50 -- Store position in percentage

function update(delta_time)
    -- Calculate movement amount based on delta time
    local move_amount = move_speed * delta_time

    -- Check arrow keys and update position
    if is_key_pressed("right") then
        block_x = block_x + move_amount
    end
    if is_key_pressed("left") then
        block_x = block_x - move_amount
    end
    if is_key_pressed("down") then
        block_y = block_y + move_amount
    end
    if is_key_pressed("up") then
        block_y = block_y - move_amount
    end

    -- Keep block within screen bounds (0-100%)
    block_x = math.max(0, math.min(100, block_x))
    block_y = math.max(0, math.min(100, block_y))

    -- Update block position
    block:location(block_x, block_y)
end

set_update_function(update)

print("Move Block Controls:")
print("Arrow Keys - Move the block")
print("Block position: " .. block_x .. ", " .. block_y)
