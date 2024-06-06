This is the repository for teo, a small and basic Brainf*ck interpreter written in C++.




It aims to replace tob, my previous Brainf*ck interpreter written in C++, why ?




teo is wayyyyy faster, for example, on my MacBook Air M1 it runs the mandelbrot.bf program in 13 seconds.



While tob runs it in 1 minute & 37 seconds.




Why did you write another BF interpreter ? It's useless.





Yep it is but I was very disatisfied with the speed of tob.





Why teo is faster than tob ?



First, tob uses std::map to simulate infinite memory, map/hashmap are really slow, teo uses an array.


Secondly, teo is using an idea that came from Tsoding(a russian dev/streamer I really like).


You combine operations so if the program contains 4 + operations and 2 < operations, you'll end up interpreting only one + operation and m_memory[m_ptr] will be incremented by 4 only once.


And the same for the < operation.



I hope you'll find the project cool ! :)



Copyright (c) 2024 - Yann BOYER
