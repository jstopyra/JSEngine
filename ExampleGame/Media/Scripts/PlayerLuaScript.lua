--PlayerLuaScript

function OnMouseInput(object, posX, posY, button)
	
end

function OnKeyboardInput(input, object, pressState)
	 if(input == "a") then    
        if(pressState == "press") then
			object:CppPlayAnimation("MoveLeft")
			object:CppSetVelocity(-1, 0, 0)
		else
			object:CppSetVelocity(1, 0, 0)
		end
	elseif(input == "s") then    
        if(pressState == "press") then
			object:CppPlayAnimation("MoveDown")
			object:CppSetVelocity(0, 1, 0)
		else
			object:CppSetVelocity(0, -1, 0)
		end
	elseif(input == "d") then    
        if(pressState == "press") then
			object:CppPlayAnimation("MoveRight")
			object:CppSetVelocity(1, 0, 0)
		else
			object:CppSetVelocity(-1, 0, 0)
		end
	elseif(input == "w") then    
        if(pressState == "press") then
			object:CppPlayAnimation("MoveUp")
			object:CppSetVelocity(0, -1, 0)
		else
			object:CppSetVelocity(0, 1, 0)
		end
	end
    
	if(pressState == "release")then
		object:CppResetAnimation()
	end
end