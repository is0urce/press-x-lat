-- name: melee.lua
-- type: lua script
-- auth: is0urce
-- desc: skill script

melee = {
	tag = "sk_melee",
	name = "Melee",
	short = "Melee",
	description = "Do 100% weapon damage",

	hostile = true,
	targeted = true,
	cooldown = 0,

	cost = function(user, target)
		return 0
	end,

	range = function(user, target)
		return 0, 1
	end,

	condition = function(user, target)
		return user ~= target
			and not user:dead()
			and not target:dead()
			and user:energy() >= melee.cost(user, target) -- have mana
			-- and game.reputation(user, target) < 0 -- friend-or-foe check, negative is hostile
			-- and game.distance(user, target) <= 1 -- melee distance
	end,

	action = function(user, target)
		local dmg = user:weapon_damage()

		target:damage(math.floor(dmg * 1.0))
	end
}