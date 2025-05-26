export imt addNums = (...numbers: number)
    mut final = 0;
    forEach(numbers, (num: number, i: number)
        final += num;
    end)
    return final;
end