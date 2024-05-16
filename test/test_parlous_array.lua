--local parray = require("ParlousArray/ParlousArray")

function Setup_Hex_Map(length, type_size)
  local arr = parlous_wfc.new_hex_map(length, type_size, 5, 5, 17)
  print("-- parlous_wfc -------------------------")
  for k,v in pairs(getmetatable(arr)) do print(k,v) end
  print("----------------------------------------")

  return arr
end

function Test_Parlous_WFC_Foreach()
  print("\n\nTest_Parlous_WFC_Foreach")
  local hex_map = Setup_Hex_Map(20, 8)
  hex_map[1] = 1; hex_map[2] = 2; hex_map[3] = 3; hex_map[4] = 4; hex_map[5] = 5;
  hex_map[6] = 6; hex_map[7] = 7; hex_map[8] = 8; hex_map[9] = 9; hex_map[0] = 10;
  assert(hex_map[0] == 10, "Expected hex_map[0]=10, actually hex_map[0]="..hex_map[0])
  print("hex_map[0] = "..hex_map[0])
  assert(hex_map[9] == 9, "Expected hex_map[9]=9, actually hex_map[9]="..hex_map[9])
  print("hex_map[9] = "..hex_map[9])
  local sum = 0
  hex_map:foreach(function (i, x) sum = sum + x end)
  print("sum = "..sum)
  assert(sum == 55, "Expected sum=55, actually sum="..sum)
end

function PPrint_Hex_Map(hex_map)
  for i=1, #hex_map do
    print("hex_map["..(i-1).."] = "..hex_map[i-1])
  end
end

function Test_Parlous_WFC_Gen()
  print("\n\nTest_Parlous_WFC_Gen")
  local hex_map = Setup_Hex_Map(40, 8)
  hex_map:map(function (i, x) return 0 end)
  hex_map:gen(function (i, x) return i end)
  hex_map:pprint()
end

print("> Test_Parlous_Wave_Function_Collapse <")
Test_Parlous_WFC_Foreach()
Test_Parlous_WFC_Gen()
