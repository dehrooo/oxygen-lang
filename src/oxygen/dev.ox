import { input } from "zzz/stdin";
import { addNums } from "modules/example.zzz";

imt hello = "Hello, ";

print("What is your name?\n");
imt name = await input(); // infers as string?

print(hello..name ?? "you");
clear(name, hello);

print("Write as many numbers as you want (separate with space):\n")
mut numbers = await input();

destroy(input);

if numbers then
    numbers
        .separate(" ")
        .toNumber()

    imt result = addNums(numbers)
    print(result);
    destroy(numbers, result, addNums);
end