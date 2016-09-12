function condition(user, body)
	return user ~= body
end

function action(user, target)
	dmg = weapon_damage(user)
	dmg = dmg * 0.2

	do_damage(target, math.floor(dmg))
end

function cost(user, target)
	return 50;
end