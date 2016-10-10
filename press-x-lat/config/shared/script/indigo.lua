-- name: indigo.lua
-- type: lua script
-- auth: is0urce
-- desc: skill script

indigo = {
	tag = "sk_indigo",
	name = "Indigo Vial",
	short = "IndigoV",
	description = "Restore 40% of maximum energy",

	hostile = false,
	targeted = false,
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
		local mana = math.floor(user:energy_max() * 0.4)
		user:replenish(mana)
	end
}