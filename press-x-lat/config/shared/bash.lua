-- name: bash.lua
-- type: lua script
-- auth: is0urce
-- desc: skill script

bash = {}

bash.cost = function(user, target)
	return 50;
end

bash.condition = function(user, target)
	return user ~= target
		and not unit.dead(target)
		and bash.cost(user, target) <= unit.energy(user) -- have mana
		-- and game.reputation(user, target) < 0 -- friend-or-foe check, negative is hostile
		-- and game.distance(user, target) <= 1 -- melee distance
end

bash.action = function(user, target)
	dmg = unit.weapon_damage(user)
	dmg = dmg * 0.25

	unit.damage(target, math.floor(dmg))
end