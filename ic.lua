#!/usr/bin/env lua

if #arg ~= 1 then
	print("ic STR")
	os.exit(255)
end

line=io.read()

while line do
	if string.find(line, arg[1]) then
		os.exit(0)
	end
	line=io.read()
end

os.exit(1)
