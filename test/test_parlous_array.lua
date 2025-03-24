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

function PPPrint(hex_map)
  hex_map:foreach(
    function (i, x)
      if i % 10 == 0 then io.write('\n') end
      if x ~= 0 then io.write(hex_map.bit(-x - 1)..'\t')  -- math.sqrt
      else io.write('.\t') end
    end)
  print()
end

function Even_PPPrint(hex_map, WIDTH)
  hex_map:foreach(
    function (i, x)
      if i % WIDTH == 0 then io.write('\n\n') end
      if i % WIDTH == 0 and math.floor(i/WIDTH)%2 == 0 then io.write('  ') end
      if x ~= 0 then
        local val = hex_map.bit(-x - 1)
        io.write(val..'   ')
        if (val < 10) then io.write(' ') end
      else io.write('.    ') end
    end)
  print()
end

function PPrint_Hex_Map(hex_map)
  for i=1, #hex_map do
    print("hex_map["..tostring(i-1).."] = "..tostring(hex_map[i-1]))
  end
end

function Test_Parlous_WFC_Gen()
  print("\n\nTest_Parlous_WFC_Gen")
  local hex_map = Setup_Hex_Map(80, 8)
  hex_map:map(function (x) return 0 end)
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

function Test_Mask()
  print("\n\nTest_Mask")
  local hex_map = Setup_Hex_Map(10, 8)
  hex_map[1] = 1; hex_map[2] = 2; hex_map[3] = 3; hex_map[4] = 4; hex_map[5] = 5;
  hex_map[6] = 6; hex_map[7] = 7; hex_map[8] = 8; hex_map[9] = 9; hex_map[0] = 10;
  assert(hex_map:mask(1, hex_map[1]) > 0, "Expected value of 1, actually "..tostring(hex_map:mask(1, hex_map[1])))
  assert(hex_map:mask(2, hex_map[2]) > 0, "Expected value of 2, actually "..tostring(hex_map:mask(2, hex_map[2])))
  assert(hex_map:mask(8, hex_map[8]) > 0, "Expected value of 8, actually "..tostring(hex_map:mask(8, hex_map[8])))
  assert(hex_map:mask(4, hex_map[8]) == 0, "Expected value of 0, actually "..tostring(hex_map:mask(4, hex_map[8])))
  assert(hex_map:mask(5, hex_map[8]) == 0, "Expected value of 0, actually "..tostring(hex_map:mask(5, hex_map[8])))
  assert(hex_map:mask(1, hex_map[9]) > 0, "Expected value of 1, actually "..tostring(hex_map:mask(1, hex_map[9])))
  assert(hex_map:mask(2, hex_map[6]) > 0, "Expected value of 2, actually "..tostring(hex_map:mask(2, hex_map[6])))
end

function Test_Or()
  print("\n\nTest_Or")
  local hex_map = Setup_Hex_Map(10, 8)
  hex_map[1] = 1; hex_map[2] = 2; hex_map[3] = 3; hex_map[4] = 4; hex_map[5] = 5;
  hex_map[6] = 6; hex_map[7] = 7; hex_map[8] = 8; hex_map[9] = 9; hex_map[0] = 10;
  assert(hex_map:Or(1, hex_map[1]) == 1, "Expected value of 1, actually "..tostring(hex_map:Or(1, hex_map[1])))
  assert(hex_map:Or(2, hex_map[2]) == 2, "Expected value of 2, actually "..tostring(hex_map:Or(2, hex_map[2])))
  assert(hex_map:Or(8, hex_map[8]) == 8, "Expected value of 8, actually "..tostring(hex_map:Or(8, hex_map[8])))
  assert(hex_map:Or(4, hex_map[8]) == 12, "Expected value of 12, actually "..tostring(hex_map:Or(4, hex_map[8])))
  assert(hex_map:Or(5, hex_map[8]) == 13, "Expected value of 13, actually "..tostring(hex_map:Or(5, hex_map[8])))
  assert(hex_map:Or(1, hex_map[9]) == 9, "Expected value of 9, actually "..tostring(hex_map:Or(1, hex_map[9])))
  assert(hex_map:Or(3, hex_map[6]) == 7, "Expected value of 7, actually "..tostring(hex_map:Or(2, hex_map[6])))
end

function Test_Not()
  print("\n\nTest_Not")
  local hex_map = Setup_Hex_Map(10, 8)
  hex_map[1] = 1; hex_map[2] = 2; hex_map[3] = 3; hex_map[4] = 4; hex_map[5] = 5;
  hex_map[6] = 6; hex_map[7] = 7; hex_map[8] = 8; hex_map[9] = 9; hex_map[0] = 10;
  assert(hex_map:Not(hex_map[3]) == -4, "Expected value of -4, actually "..tostring(hex_map:Not(8, hex_map[8])))
  assert(hex_map:Not(hex_map[8]) == -9, "Expected value of -9, actually "..tostring(hex_map:Not(4, hex_map[8])))
  assert(hex_map:Not(hex_map[6]) == -7, "Expected value of -7, actually "..tostring(hex_map:Not(2, hex_map[6])))
end

function Test_Xor()
  print("\n\nTest_Xor")
  local hex_map = Setup_Hex_Map(10, 8)
  hex_map[1] = 1; hex_map[2] = 2; hex_map[3] = 3; hex_map[4] = 4; hex_map[5] = 5;
  hex_map[6] = 6; hex_map[7] = 7; hex_map[8] = 8; hex_map[9] = 9; hex_map[0] = 10;
  assert(hex_map:Xor(1, hex_map[1]) == 0, "Expected value of 0, actually "..tostring(hex_map:Xor(1, hex_map[1])))
  assert(hex_map:Xor(2, hex_map[2]) == 0, "Expected value of 0, actually "..tostring(hex_map:Xor(2, hex_map[2])))
  assert(hex_map:Xor(4, hex_map[8]) == 12, "Expected value of 12, actually "..tostring(hex_map:Xor(4, hex_map[8])))
  assert(hex_map:Xor(5, hex_map[8]) == 13, "Expected value of 13, actually "..tostring(hex_map:Xor(5, hex_map[8])))
  assert(hex_map:Xor(1, hex_map[9]) == 8, "Expected value of 8, actually "..tostring(hex_map:Xor(1, hex_map[9])))
  assert(hex_map:Xor(3, hex_map[6]) == 5, "Expected value of 5, actually "..tostring(hex_map:Xor(2, hex_map[6])))
end

function Test_Bit()
  print("\n\nTest_Bit")
  local hex_map = Setup_Hex_Map(10, 8)
  hex_map[1] = 1; hex_map[2] = 2; hex_map[3] = 3; hex_map[4] = 4; hex_map[5] = 5;
  hex_map[6] = 6; hex_map[7] = 8; hex_map[8] = 7; hex_map[9] = 9; hex_map[0] = 10;
  assert(hex_map.bit(hex_map[1]) == 1, "Expected value of 1, actually "..tostring(hex_map:bit(hex_map[1])))
  assert(hex_map.bit(hex_map[2]) == 2, "Expected value of 2, actually "..tostring(hex_map:bit(hex_map[2])))
  assert(hex_map.bit(hex_map[7]) == 4, "Expected value of 4, actually "..tostring(hex_map:bit(hex_map[8])))
end

function Test_Bit_Count()
  print("\n\nTest_Bit_Count")
  local hex_map = Setup_Hex_Map(10, 8)
  hex_map[1] = 1; hex_map[2] = 2; hex_map[3] = 3; hex_map[4] = 4; hex_map[5] = 5;
  hex_map[6] = 6; hex_map[7] = 8; hex_map[8] = 7; hex_map[9] = 9; hex_map[0] = 10;
  assert(hex_map.bit_count(hex_map[1]) == 1, "Expected value of 1, actually "..tostring(hex_map:bit_count(hex_map[1])))
  assert(hex_map.bit_count(hex_map[2]) == 1, "Expected value of 1, actually "..tostring(hex_map:bit_count(hex_map[2])))
  assert(hex_map.bit_count(hex_map[8]) == 3, "Expected value of 3, actually "..tostring(hex_map:bit_count(hex_map[8])))
  assert(hex_map.bit_count(hex_map[3]) == 2, "Expected value of 2, actually "..tostring(hex_map:bit_count(hex_map[8])))
  assert(hex_map.bit_count(hex_map[7]) == 1, "Expected value of 1, actually "..tostring(hex_map:bit_count(hex_map[8])))
  assert(hex_map.bit_count(hex_map[9]) == 2, "Expected value of 2, actually "..tostring(hex_map:bit_count(hex_map[8])))
end

function Test_Circle_Placement()
  print("\n\nTest_Circle_Placement")
  local hex_map = Setup_Hex_Map(100, 8)
  hex_map:map(function (x) return 0 end)
  local idx = 0
  local found = 0
  local foundAny = 0
  local foundAll = 0
  hex_map:gen(
  function (hx, x, newWave, dist, cir)
    idx = idx + 1
    return 2^((idx-1)%120)
  end)
  Even_PPPrint(hex_map, 10)
  -- hex_map:foreach(
  --   function (i, x)
  --     if i % 10 == 0 then io.write('\n') end
  --     if x ~= 0 then io.write(hex_map.bit(-x - 1)..'\t')  -- math.sqrt
  --     else io.write('.\t') end
  --   end)
  -- print()  -- ### DEBUG
end

function Test_HexMapHelper_function()
  print("\n\nTest_HexMapHelper_function")
  local hex_map = Setup_Hex_Map(100, 8)
  hex_map:map(function (x) return 0 end)
  local idx = 0
  local found1 = 0
  local found2 = 0
  local found3 = 0
  local found4 = 0
  local found5 = 0
  local found6 = 0
  local foundXLoopLeft = 0
  local foundXLoopRight = 0
  local foundYLoopUp = 0
  local foundYLoopDown = 0
  hex_map:gen(
  function (hx, x, newWave, dist, cir)
    idx = idx + 1
    if hx(-1,0) == 1 then found1 = idx end
    if hx(0,-1) == 2 then found2 = idx end
    if hx(1,-1) == 2^26 then found3 = idx end
    if hx(1,0) == 2^4 then found4 = idx end
    if hx(0,1) == 2^4 then found5 = idx end
    if hx(-1,1) == 2^2 then found6 = idx end
    if hx(-5,0) == 2^9 then foundXLoopLeft = idx end
    if hx(-5,0) == 2^5 then foundXLoopRight = idx end
    if hx(0,-5) == 2^8 then foundYLoopUp = idx end
    if hx(0,5) == 2^9 then foundYLoopDown = idx end
    -- io.write('['..idx..': '..hex_map.bit(hx(-1,0))..'], ')  -- ### DEBUG
    return 2^((idx-1)%120)
  end)
  Even_PPPrint(hex_map, 10)
  -- PPPrint(hex_map)
  -- hex_map:foreach(
  --   function (i, x)
  --     if i % 10 == 0 then io.write('\n') end
  --     if x ~= 0 then io.write(hex_map.bit(-x - 1)..'\t')  -- math.sqrt
  --     else io.write('.\t') end
  --   end)
  -- print()
  assert(found1 == 3, "Expected value of 3, actually "..tostring(found1))
  assert(found2 == 3, "Expected value of 3, actually "..tostring(found2))
  assert(found3 == 28, "Expected value of 28, actually "..tostring(found3))
  assert(found4 == 16, "Expected value of 16, actually "..tostring(found4))
  assert(found5 == 35, "Expected value of 35, actually "..tostring(found5))
  assert(found6 == 8, "Expected value of 8, actually "..tostring(found6))
  assert(foundXLoopLeft == 25, "Expected value of 25, actually "..tostring(foundXLoopLeft))
  assert(foundXLoopRight == 35, "Expected value of 35, actually "..tostring(foundXLoopRight))
  assert(foundYLoopUp == 11, "Expected value of 11, actually "..tostring(foundYLoopUp))
  assert(foundYLoopDown == 12, "Expected value of 12, actually "..tostring(foundYLoopDown))
end

function Test_Circle()
  print("\n\nTest_Circle")
  print("----------------------------------------")
  local WIDTH = 10 -- 12 -- 20

  -- Initialize hex_map
  local hex_map = parlous_wfc.new_hex_map(WIDTH*WIDTH, 8, WIDTH, 5, 17)
  local idx = 0
  hex_map:map(function (x)
    idx = idx + 1
    if idx == math.floor(WIDTH*WIDTH/2) + math.floor(WIDTH/2) then return hex_map.Not(2^7) end
    return 0 end)
  -- PPPrint(hex_map)
  hex_map:foreach( function (i, x) io.write('['..i..']: '..x..', ') end)

  -- Define function to look for set hex cell (bit = 8)s
  function Look_for_8(v, bit_count, bit)
    if bit_count(v) ~= 1 then  -- ### DEBUG
      if v == -1 then io.write('*, ')  -- ### DEBUG
      else io.write(v..', ') end  -- ### DEBUG   '$, '
    else io.write(bit(v)..', ') end  -- ### DEBUG
    if bit_count(v) ~= 1 then return false end
    if bit(v) == 8 then return true else return false end
   end

  -- Generate map listing circle distances from set hex cell
  idx = 0
  local repeats = 0
  hex_map:gen(
  function (hx, x, newWave, dist, cir)
    idx = idx + 1
    io.write('\n### ['..(idx+1)..']: ')  -- ### DEBUG
    if hex_map.bit_count(x) == 1 then repeats = repeats + 1 end
    io.write('|1| ')  -- ### DEBUG
    if cir(1):any(Look_for_8) then return 1 end
    io.write('|2| ')  -- ### DEBUG
    if cir(2):any(Look_for_8) then return 2 end
    io.write('|3| ')  -- ### DEBUG
    if cir(3):any(Look_for_8) then return 4 end
    io.write('|4| ')  -- ### DEBUG
    if cir(4):any(Look_for_8) then return 8 end
    io.write('|5| ')  -- ### DEBUG
    if cir(5):any(Look_for_8) then return 16 end
    return 32
  end)
  hex_map:foreach(
    function (i, x)
      if i % WIDTH == 0 then io.write('\n\n') end
      if i % WIDTH == 0 and math.floor(i/WIDTH)%2 == 0 then io.write('  ') end
      if x ~= 0 then io.write(hex_map.bit(-x - 1)..'    ')
      else io.write('.    ') end
    end)
  print()
  assert(repeats == 0, "Expected value of 0, actually "..tostring(repeats))
end

function Test_Circle_Any()
  print("\n\nTest_Circle_Any")
  local hex_map = Setup_Hex_Map(100, 8)
  hex_map:map(function (x) return 0 end)
  local idx = 0
  local foundAny1 = 0
  local foundAny2 = 0
  local foundAny3 = 0
  hex_map:gen(function (hx, x, newWave, dist, cir)
    idx = idx + 1
    if idx == 8 then return 8 end
    if cir(1):any(function(v, bit_count, bit)
      if bit_count(v) ~= 1 then return false end
      -- print("bit(v) = "..bit(v).."\tv = "..v)  -- ### DEBUG
      if bit(v) == 4 then return true else return false end
    end) then foundAny1 = foundAny1 + 1 end
    if cir(2):any(function(v, bit_count, bit)
      if bit_count(v) ~= 1 then return false end
      if bit(v) == 4 then return true else return false end
    end) then foundAny2 = foundAny2 + 1 end
    if cir(3):any(function(v, bit_count, bit)
      if bit_count(v) ~= 1 then return false end
      -- if bit(v) == 4 then print("["..idx.."]:\tbit(v) = "..bit(v).."\tv = "..v) end  -- ### DEBUG
      if bit(v) == 4 then return true else return false end
    end) then foundAny3 = foundAny3 + 1 end
    return 4
  end)
  Even_PPPrint(hex_map, 10)
  -- PPPrint(hex_map)
  print("foundAny1 = "..foundAny1)  -- ### DEBUG
  assert(foundAny1 == 5, "Expected value of 5, actually "..tostring(foundAny1))
  print("foundAny2 = "..foundAny2)  -- ### DEBUG
  assert(foundAny2 == 8, "Expected value of 8, actually "..tostring(foundAny2))
  print("foundAny3 = "..foundAny3)  -- ### DEBUG
  assert(foundAny3 == 8, "Expected value of 8, actually "..tostring(foundAny3))
end

function Test_Circle_All()
  print("\n\nTest_Circle_All")
  local hex_map = Setup_Hex_Map(100, 8)
  hex_map:map(function (x) return 0 end)
  local idx = 0
  local foundAll1 = 0
  local foundAll2 = 0
  hex_map:gen(function (hx, x, newWave, dist, cir)
    idx = idx + 1
    if idx == 8 then return 8 end
    if cir(1):all(function(v, bit_count, bit)
      if bit_count(v) ~= 1 then return false end
      if bit(v) == 4 then return true else return false end
    end) then foundAll1 = foundAll1 + 1 end
    return 4
  end)
  PPPrint(hex_map); print()
  print("foundAll1 = "..foundAll1)  -- ### DEBUG
  assert(foundAll1 == 36, "Expected value of 36, actually "..tostring(foundAll1))
end

function Test_Gen_NewWave_Selection()
  print("\n\nTest_Gen_NewWave_Selection")
  local hex_map = Setup_Hex_Map(80, 8)
  hex_map:map(function (x) return 0 end)
  local calls = 0
  hex_map:gen(function (hx, x, newWave, dist)
    if (newWave)
    then
      calls = calls + 1;
      return 2^(calls - 1)
    end
    return x
  end)
  Even_PPPrint(hex_map, 10)
  -- hex_map:pprint()
  assert(hex_map[34] == -2, "Expected value of -2, actually "..tostring(hex_map[34]))
  assert(hex_map[22] == -3, "Expected value of -3, actually "..tostring(hex_map[22]))
  assert(hex_map[56] == -5, "Expected value of -5, actually "..tostring(hex_map[56]))
  assert(hex_map[27] == -9, "Expected value of -9, actually "..tostring(hex_map[27]))
  assert(hex_map[44] == -17, "Expected value of -17, actually "..tostring(hex_map[44]))
  assert(calls == 24, "Expected value of 24, actually "..tostring(calls))
  print("Number of calls to gen: "..calls)
end

function Test_Parlous_WFC_Gen_Time()
  print("\n\nTest_Parlous_WFC_Gen_Time")
  local hex_map = parlous_wfc.new_hex_map(480000, 8, 600, 5, 17)
  hex_map:map(function (x) return 0 end)
  local calls = 0
  local start_time = os.time()
  hex_map:gen(function (hx, x, newWave, dist)
    calls = calls + 1;
    if (newWave) then return 16 end
    if hex_map:mask(1, hx(0, -1)) == 1 then return 2^1 end
    if hex_map:mask(2, hx(0, 1)) == 1 then return 2^2 end
    return 2^3
  end)
  print("Time tanspired building table: "..os.time()-start_time)
  print("Number of calls to gen: "..calls)
  assert(hex_map[1615] == -17, "Expected value of -17, actually "..tostring(hex_map[34]))
  assert(calls <= 480000, "Expected value of less than 480000, actually "..tostring(calls))
end


print("> Test_Parlous_Wave_Function_Collapse <")
Test_Parlous_WFC_Foreach()
Test_Parlous_WFC_Gen()
Test_Mask()
Test_Or()
Test_Not()
Test_Xor()
Test_Bit()
Test_Bit_Count()
Test_Circle_Placement()
Test_HexMapHelper_function()
Test_Circle()
Test_Circle_Any()
Test_Circle_All()
Test_Gen_NewWave_Selection()
Test_Parlous_WFC_Gen_Time()
