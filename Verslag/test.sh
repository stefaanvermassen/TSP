#!/bin/bash
#snelheidstest voor heuristieken
for ((i = 3 ; i < 50 ; i++)); do
	all_avg5=""
	all_avg10=""
	all_avg15=""
	heuristics_avg=""
	for ((j = 0 ; j < 3 ; j++)); do
		python generate_world.py $i > "invoer_${i}.txt"
		ts=$(date +%s%N) ; ./greedy.out "invoer_${i}.txt" ; tt=$((($(date +%s%N) - $ts)/1000000))
		heuristics_avg="${heuristics_avg}${tt}+"
		ts=$(date +%s%N) ; mpirun -n 1 all.out "invoer_${i}.txt" ; tt=$((($(date +%s%N) - $ts)/1000000))
		all_avg5="${all_avg5}${tt}+"
		ts=$(date +%s%N) ; mpirun -n 2 all.out "invoer_${i}.txt" ; tt=$((($(date +%s%N) - $ts)/1000000))
                all_avg10="${all_avg10}${tt}+"
		ts=$(date +%s%N) ; mpirun -n 5 all.out "invoer_${i}.txt" ; tt=$((($(date +%s%N) - $ts)/1000000))
                all_avg15="${all_avg15}${tt}+"
		rm "invoer_${i}.txt"
	done
	all_avg5=$(echo "$all_avg5" | sed 's/\(.*\)+/scale=3;(\1)\/3/' | bc)
	all_avg10=$(echo "$all_avg10" | sed 's/\(.*\)+/scale=3;(\1)\/3/' | bc)
	all_avg15=$(echo "$all_avg15" | sed 's/\(.*\)+/scale=3;(\1)\/3/' | bc)
	heuristics_avg=$(echo "$heuristics_avg" | sed 's/\(.*\)+/scale=3;(\1)\/3/' | bc)
	echo -n "${i};" >> times_heuristics.dat
	echo "$heuristics_avg" >> times_heuristics.dat
	echo -n "${i};" >> times_all5.dat
        echo "$all_avg5" >> times_all5.dat
	echo -n "${i};" >> times_all10.dat
        echo "$all_avg10" >> times_all10.dat
        echo -n "${i};" >> times_all15.dat
        echo "$all_avg15" >> times_all15.dat
done
#afwijking van de optimale kost
for ((i = 3 ; i < 25 ; i++)); do
	python generate_world.py $i > "invoer_${i}.txt"
	echo -n "${i};" >> costs.dat
	mpirun -n 4 all.out "invoer_${i}.txt" | tr '\n' ';' | sed 's/;.*/;/' >> costs.dat
	./greedy.out "invoer_${i}.txt" >> costs.dat
	rm "invoer_${i}.txt"
done
#testen van de splitsdiepte
#voor deze test moet een extra parameter gevraagd worden in het algoritme, namelijk de splitsdiepte
#!/bin/bash
for ((i = 20 ; i < 31 ; i++)); do
	python generate_world.py $i > invoer.txt
	for ((p = 2 ; p < 5 ; p++)); do
        	for ((d = 1 ; d < 11 ; d++)); do
                	echo -n "$d;" >> "splitdata_${p}_${i}.dat"
                	ts=$(date +%s%N) ; mpirun -n $p tsp.out invoer.txt $d ; tt=$((($(date +%s%N) - $ts)/1000000)); echo $tt >> "splitdata_${p}_${i}.dat"
        	done
	done
done
