-- name: teleport.lua
-- type: lua script
-- auth: is0urce
-- desc: skill script

teleport = {
	tag = "sk_teleport",
	name = "Teleport",
	short = "Tport",
	description = "Teleports you to target location",

	hostile = false,
	targeted = false, -- target location, not units
	cooldown = 20,

	cost = function(user, target)
		return 0
	end,

	range = function(user, target)
		return 0, 10000
	end,

	condition = function(user, target)
		return not user:dead()
	end,

	action = function(user, target)
		user:move(target)
	end
}