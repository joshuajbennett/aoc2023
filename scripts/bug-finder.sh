mkdir Release
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j23
polyspace-configure -compilation-database ./compile_commands.json -output-options-file options.txt -allow-overwrite
polyspace-bug-finder -options-file options.txt -results-dir ./static_analysis_results/

