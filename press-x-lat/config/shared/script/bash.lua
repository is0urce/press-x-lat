-- name: bash.lua
-- type: lua script
-- auth: is0urce
-- desc: skill script

bash = {
	tag = "sk_bash",
	name = "Bash",
	description = "Do 100% weapon damage, plus 100% blunt damage",

	hostile = true,
	targeted = true,

	cost = function(user, target)
		return 5
	end,

	range = function(user, target)
		return 0, 1
	end,

	condition = function(user, target)
		return user ~= target
			and not user:dead()
			and not target:dead()
			and user:energy() >= bash.cost(user, target) -- have mana
			-- and game.reputation(user, target) < 0 -- friend-or-foe check, negative is hostile
			-- and game.distance(user, target) <= 1 -- melee distance
	end,

	action = function(user, target)
		local dmg = user:weapon_damage()

		target:damage(math.floor(dmg * 1.0))
		target:damage(math.floor(dmg * 1.0))
		user:drain(bash.cost(user, target))
	end
}