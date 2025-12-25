local utils = {}

local defaultFmt = "[%s] : %s"

utils.Types = {}

---@class utils_Script
utils.Types.Script = {
	---@type integer
	previousSize = nil,
	---@type file*?
	currentFile = nil,
	brokenMsg = false,
	missingMsg = false,
	---@type string
	path = nil,
	---@type string
	name = nil,
	lastRet = nil,

	new = __MakeClass,

	---@param class utils_Script
	---@param bTable utils_Script
	init = function(class, bTable, path, name)
		__MakeClass(class, bTable)

		bTable.path = path
		bTable.name = name

		return bTable
	end,

	---@param self utils_Script
	---@return boolean
	execute = function(self, ...)
		local code, err = loadstring(self.currentFile:read("a"))

		if (not code) then
			if (not self.brokenMsg) then
				self.brokenMsg = true

				print(string.format(defaultFmt, self.path, err))
			end

			return false
		end

		self.brokenMsg = false

		self.lastRet = code()

		-- Reconsider if function returning should be mandatory
		if (type(self.lastRet) == "function") then
			local res, funcErr = pcall(self.lastRet, ...)

			if (not res) then
				print(string.format(defaultFmt, self.path, funcErr))

				return false
			end

			return true
		end

		return false
	end,

	---@param self utils_Script
	---@return boolean
	update = function(self, ...)
		if (not self.currentFile) then
			self.currentFile = io.open(self.path, "r")

			if (not self.currentFile) then
				if (not self.missingMsg) then
					self.missingMsg = true

					print(string.format("\"%s\" cannot be opened", self.path))
				end

				return false
			end
		end

		self.missingMsg = false

		local size = self.currentFile:seek("end")

		if (size == nil) then
			self.currentFile:close()
			self.currentFile = nil

			print(string.format("\"%s\" cannot be accessed anymore", self.path))

			return false
		end

		self.currentFile:seek("set")

		if (self.previousSize ~= size) then
			-- It reads the previous state of the file if not reopened
			self.currentFile:close()
			self.currentFile = io.open(self.path, "r")

			if (not self.currentFile) then
				self.previousSize = nil

				if (not self.missingMsg) then
					self.missingMsg = true

					print(string.format("\"%s\" cannot be reopened", self.path))
				end

				return false
			end

			self.previousSize = size

			return self:execute(...)
		end

		return false
	end,
}

---@return boolean
utils.cStrEql = function(s1, s2)
	local i = 0
	local c1 = 0
	local c2 = 0

	repeat
		c1 = s1[i]
		c2 = s2[i]

		if (c1 ~= c2) then
			return false
		end

		if (c1 == 0) then
			break
		end

		i = i + 1
	until false

	return true
end

---@return boolean
---@param luaStr string
utils.cLuaStrEql = function(cStr, luaStr)
	local i = 0
	local c1 = 0
	local c2 = 0

	repeat
		c1 = cStr[i]
		c2 = luaStr:byte(i + 1) or 0

		if (c1 ~= c2) then
			return false
		end

		if (c1 == 0) then
			break
		end

		i = i + 1
	until false

	return true
end

-- struct must be a struct, not a pointer
utils.memberIndex = function(struct, memberName)
	return (ffi.offsetof(struct, memberName) / ffi.sizeof(struct[memberName]))
end

---@param t table
utils.tableLen = function(t)
	local i = 0

	for _ in pairs(t) do
		i = i + 1
	end

	return i
end

utils.memoize = function(cache, keypos, func)
	return function(...)
		local args = { ... }
		local key = args[keypos]

		local cachedRes = cache[key]

		if (cachedRes) then
			return cachedRes
		end

		cachedRes = func(...)

		cache[key] = cachedRes

		return cachedRes
	end
end

return utils
