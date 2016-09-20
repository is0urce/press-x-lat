-- name: damage.lua
-- type: lua script
-- auth: is0urce
-- desc: enumerate damage types

damage = {
	pure = 0, -- no-resist type

	-- Etheral / Material
	piercing = 1,
	slashing = 2,
	blunt = 3,
	gravitation = 4, -- laws of physics, fall damage

	-- Arcane / Mastery
	arcane = 5,
	curse = 6,
	anomaly = 7, -- radiation, temporal
	disintegration = 8, -- mechanical

	-- Will / Mind
	inexplicable = 9,
	sonic = 10,
	magnetic = 11,
	mental = 12, -- psionic

	-- Chaos / Law
	ice = 13,
	acid = 14,
	shock = 15, -- lighting
	fire = 16,

	-- Death / Life
	light = 17,
	poison = 18,
	disease = 19,
	darkness 20
}