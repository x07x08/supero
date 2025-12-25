local asy = {}

---@diagnostic disable-next-line
local __unpack = table.unpack or unpack

asy.ns_per_us = 1000
asy.ns_per_ms = 1000 * asy.ns_per_us
asy.ns_per_s = 1000 * asy.ns_per_ms
asy.ns_per_min = 60 * asy.ns_per_s
asy.ns_per_hour = 60 * asy.ns_per_min
asy.ns_per_day = 24 * asy.ns_per_hour
asy.ns_per_week = 7 * asy.ns_per_day

asy.us_per_ms = 1000
asy.us_per_s = 1000 * asy.us_per_ms
asy.us_per_min = 60 * asy.us_per_s
asy.us_per_hour = 60 * asy.us_per_min
asy.us_per_day = 24 * asy.us_per_hour
asy.us_per_week = 7 * asy.us_per_day

asy.ms_per_s = 1000
asy.ms_per_min = 60 * asy.ms_per_s
asy.ms_per_hour = 60 * asy.ms_per_min
asy.ms_per_day = 24 * asy.ms_per_hour
asy.ms_per_week = 7 * asy.ms_per_day

asy.s_per_min = 60
asy.s_per_hour = asy.s_per_min * 60
asy.s_per_day = asy.s_per_hour * 24
asy.s_per_week = asy.s_per_day * 7

---@class asy_EVLTimer
asy.EVLTimer = {
	instance = nil,

	new = __MakeClass,

	---@param class asy_EVLTimer
	---@param bTable asy_EVLTimer
	init = function(class, bTable)
		__MakeClass(class, bTable)

		bTable.instance = ffi.gc(CExt.self.cexp_startTimer(), CExt.self.cexp_freeTimer)

		return bTable
	end,

	---@param self asy_EVLTimer
	read = function(self)
		return CExt.self.cexp_readTimer(self.instance)
	end,

	---@param self asy_EVLTimer
	lap = function(self)
		return CExt.self.cexp_lapTimer(self.instance)
	end,

	---@param self asy_EVLTimer
	reset = function(self)
		CExt.self.cexp_resetTimer(self.instance)
	end,
}

---@class asy_TimedFunc
asy.TimedFunc = {
	---@type function
	func = nil,
	delay = 0,
	timestamp = 0,
	---@type table
	args = nil,

	new = __MakeClass,
}

asy.FuncClassTypes = {
	Periodic = 0,
	Timeout = 1,
}

---@class asy_PeriodicFunc : asy_TimedFunc
asy.Periodic = asy.TimedFunc:new({})
asy.Periodic.__classtype = asy.FuncClassTypes.Periodic

---@class asy_TimeoutFunc : asy_TimedFunc
asy.Timeout = asy.TimedFunc:new({})
asy.Timeout.__classtype = asy.FuncClassTypes.Timeout

---@class asy_EventLoop
asy.EventLoop = {
	---@type (asy_TimedFunc | asy_PeriodicFunc | asy_TimeoutFunc)[]
	funcs = nil,
	---@type asy_EVLTimer
	timer = nil,
	terminate = false,
	lowestDelay = nil,
	rEvent = nil,
	started = false,

	new = __MakeClass,

	---@param class asy_EventLoop
	---@param bTable asy_EventLoop
	init = function(class, bTable)
		__MakeClass(class, bTable)

		bTable.rEvent = ffi.gc(CExt.self.cexp_allocResetEvent(), CExt.self.cexp_freeResetEvent)
		bTable.timer = asy.EVLTimer:init({})
		bTable.funcs = {}

		return bTable
	end,

	---@param self asy_EventLoop
	---@param ... any
	timeout = function(self, func, delay, ...)
		local timeoutFn = asy.Timeout:new({})

		timeoutFn.func = func
		timeoutFn.delay = delay
		timeoutFn.timestamp = self.timer:read()

		---@type table
		local argsTable = { ... }
		argsTable.__n = #argsTable
		timeoutFn.args = argsTable

		table.insert(self.funcs, timeoutFn)

		if (self.started) then
			CExt.self.cexp_setResetEvent(self.rEvent)
		end

		return timeoutFn
	end,

	---@param self asy_EventLoop
	---@param ... any
	periodic = function(self, func, delay, ...)
		local periodicFn = asy.Periodic:new({})

		periodicFn.func = func
		periodicFn.delay = delay
		periodicFn.timestamp = self.timer:read()

		---@type table
		local argsTable = { ... }
		argsTable.__n = #argsTable
		periodicFn.args = argsTable

		table.insert(self.funcs, periodicFn)

		if (self.started) then
			CExt.self.cexp_setResetEvent(self.rEvent)
		end

		return periodicFn
	end,

	---@param self asy_EventLoop
	stop = function(self)
		self.terminate = true
		CExt.self.cexp_setResetEvent(self.rEvent)
	end,

	---@param self asy_EventLoop
	start = function(self)
		self.started = true
		CExt.self.cexp_resetResetEvent(self.rEvent)

		local targetTime
		local currentDelay

		while true do
			if (self.terminate) then
				break
			end

			self.lowestDelay = nil
			local now = self.timer:read()

			for index, value in ipairs(self.funcs) do
				targetTime = value.delay + value.timestamp

				if (now >= targetTime) then
					value.func(__unpack(value.args, 1, value.args.__n))

					if (value.__classtype == asy.FuncClassTypes.Timeout) then
						table.remove(self.funcs, index)
					elseif (value.__classtype == asy.FuncClassTypes.Periodic) then
						value.timestamp = now

						if ((self.lowestDelay == nil) or (value.delay < self.lowestDelay)) then
							self.lowestDelay = value.delay
						end
					end
				else
					currentDelay = targetTime - now

					if ((self.lowestDelay == nil) or (currentDelay < self.lowestDelay)) then
						self.lowestDelay = currentDelay
					end
				end
			end

			if ((self.lowestDelay ~= nil) and (self.lowestDelay > 0)) then
				if (not CExt.self.cexp_timedWaitResetEvent(self.rEvent, self.lowestDelay)) then
					CExt.self.cexp_resetResetEvent(self.rEvent)
				end
			else
				CExt.self.cexp_waitResetEvent(self.rEvent)
			end
		end
	end,
}

return asy
