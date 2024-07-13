--local parray = require("ParlousArray/ParlousArray")

function Setup_Hex_Map(length, type_size)
  local arr = parlous_wfc.new_hex_map(length, type_size, 10, 5, 17)
  print("-- parlous_wfc -------------------------")
  for k,v in pairs(getmetatable(arr)) do print(k,v) end
  print("----------------------------------------")

  return arr
end

function Test_Parlous_WFC_Foreach()
  print("\n\nTest_Parlous_WFC_Foreach")
  local hex_map = Setup_Hex_Map(10, 8)
  hex_map[1] = 1; hex_map[2] = 2; hex_map[3] = 3; hex_map[4] = 4; hex_map[5] = 5;
  hex_map[6] = 6; hex_map[7] = 7; hex_map[8] = 8; hex_map[9] = 9; hex_map[0] = 10;
  print("-----<>------")  -- ### DEBUG
  print(hex_map[0])  -- ### DEBUG
  assert(hex_map[0] == 10, "Expected hex_map[0]=10, actually hex_map[0]="..tostring(hex_map[0]))
  print("hex_map[0] = "..tostring(hex_map[0]))
  assert(hex_map[9] == 9, "Expected hex_map[9]=9, actually hex_map[9]="..tostring(hex_map[9]))
  print("hex_map[9] = "..tostring(hex_map[9]))
  local sum = 0
  hex_map:foreach(function (i, x) sum = sum + x end)
  print("sum = "..tostring(sum))
  assert(sum == 55, "Expected sum=55, actually sum="..tostring(sum))
end

function PPrint_Hex_Map(hex_map)
  for i=1, #hex_map do
    print("hex_map["..tostring(i-1).."] = "..tostring(hex_map[i-1]))
  end
end

function Test_Parlous_WFC_Gen()
  print("\n\nTest_Parlous_WFC_Gen")
  local hex_map = Setup_Hex_Map(80, 8)
  hex_map:map(function (i, x) return 0 end)
  local calls = 0
  hex_map:gen(function (hx, x, newWave, dist)
    -- print("### ["..calls.."]: "..tostring(newWave))  -- ### DEBUG
    -- print("### ["..calls.."]:\tx = "..x)  -- ### DEBUG
    -- hx(0,1)  -- ### DEBUG

    calls = calls + 1;
    if (newWave)
    then
      print("###--> ["..calls.."] x = "..x.." -> new_x = "..2^(calls - 1))  -- ### DEBUG
      return 2^(calls - 1)
    end
    return x

    -- if (newWave) then return 2^0 end
    -- if hex_map:mask(1, hx(0, -1)) == 1 then return 2^1 end
    -- if hex_map:mask(2, hx(0, 1)) == 1 then return 2^2 end
    -- return 2^3
  end)
  hex_map:pprint()
  print("Number of calls to gen: "..calls)
end

function Test_Bit_Count()
  print("\n\nTest_Bit_Count")
  local hex_map = Setup_Hex_Map(10, 8)
  hex_map[1] = 1; hex_map[2] = 2; hex_map[3] = 3; hex_map[4] = 4; hex_map[5] = 5;
  hex_map[6] = 6; hex_map[7] = 7; hex_map[8] = 8; hex_map[9] = 9; hex_map[0] = 10;
  assert(hex_map:bit_count(hex_map[1]) == 1, "Expected value of 1, actually "..tostring(hex_map:bit_count(hex_map[1])))
  assert(hex_map:bit_count(hex_map[2]) == 1, "Expected value of 1, actually "..tostring(hex_map:bit_count(hex_map[2])))
  assert(hex_map:bit_count(hex_map[3]) == 2, "Expected value of 2, actually "..tostring(hex_map:bit_count(hex_map[3])))
  assert(hex_map:bit_count(hex_map[7]) == 3, "Expected value of 1, actually "..tostring(hex_map:bit_count(hex_map[7])))
  assert(hex_map:bit_count(hex_map[8]) == 1, "Expected value of 1, actually "..tostring(hex_map:bit_count(hex_map[8])))
end

function Test_Bit()
  print("\n\nTest_Bit")
  local hex_map = Setup_Hex_Map(10, 8)
  hex_map[1] = 1; hex_map[2] = 2; hex_map[3] = 3; hex_map[4] = 4; hex_map[5] = 5;
  hex_map[6] = 6; hex_map[7] = 7; hex_map[8] = 8; hex_map[9] = 9; hex_map[0] = 10;
  assert(hex_map:bit(hex_map[1]) == 1, "Expected value of 1, actually "..tostring(hex_map:bit(hex_map[1])))
  assert(hex_map:bit(hex_map[2]) == 2, "Expected value of 2, actually "..tostring(hex_map:bit(hex_map[2])))
  assert(hex_map:bit(hex_map[8]) == 4, "Expected value of 4, actually "..tostring(hex_map:bit(hex_map[8])))
end

function Test_Gen_NewWave_Selection()
  print("\n\nTest_Gen_NewWave_Selection")
  local hex_map = Setup_Hex_Map(80, 8)
  hex_map:map(function (i, x) return 0 end)
  local calls = 0
  hex_map:gen(function (hx, x, newWave, dist)
    if (newWave)
    then
      calls = calls + 1;
      return 2^(calls - 1)
    end
    return x
  end)
  hex_map:pprint()
  print("### "..hex_map:bit_count(hex_map[56]))  -- ### DEBUG
  print("### "..hex_map:bit(hex_map[56]))  -- ### DEBUG
  assert(hex_map[34] == -2, "Expected value of -2, actually "..tostring(hex_map[34]))
  assert(hex_map[22] == -3, "Expected value of -3, actually "..tostring(hex_map[22]))
  assert(hex_map[56] == -5, "Expected value of -5, actually "..tostring(hex_map[56]))
  assert(hex_map[27] == -9, "Expected value of -9, actually "..tostring(hex_map[27]))
  assert(hex_map[44] == -17, "Expected value of -17, actually "..tostring(hex_map[44]))
  print("Number of calls to gen: "..calls)
end

print("> Test_Parlous_Wave_Function_Collapse <")
Test_Parlous_WFC_Foreach()
Test_Parlous_WFC_Gen()
Test_Bit_Count()
Test_Bit()
Test_Gen_NewWave_Selection()
