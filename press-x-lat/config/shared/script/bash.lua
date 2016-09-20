-- name: bash.lua
-- type: lua script
-- auth: is0urce
-- desc: skill script

bash = {
	tag = "sk_bash",
	name = "Bash",
	short = "Bash",
	description = "Do 100% weapon damage, plus 100% blunt damage",

	hostile = true,
	targeted = true,
	cooldown = 0,

	cost = function(user, target)
		return 5
	end,

	range = function(user, target) -- melee distance
		return 0, 1
	end,

	condition = function(user, target)
		local distance = game.distance(user, target)
		local min, max = bash.range(user, target)
		return user ~= target
			and not user:dead()
			and not target:dead()
			and user:energy() >= bash.cost(user, target) -- have mana
			and min <= distance and distance <= max -- in range
			-- and game.reputation(user, target) < 0 -- friend-or-foe check, negative is hostile
	end,

	action = function(user, target)
		user:drain(bash.cost(user, target))

		local dmg, type, hit, crit = game.hit(user, target)
		local msg = "miss"

		if hit then
			local weapon = math.floor(dmg * 1.0)
			local bonus = math.floor(dmg * 1.0)
			local total = weapon + bonus;
			
			target:damage(weapon)
			target:damage(bonus)

			msg = -total
			if crit then
				msg = msg + " critical!"
			end
		end
		game.message(user, msg)
	end
}