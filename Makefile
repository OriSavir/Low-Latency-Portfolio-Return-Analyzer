all:
	mkdir -p portfolio_analysis_build
	cd portfolio_analysis_build && cmake ..
	cd portfolio_analysis_build && cmake --build .

clean:
	rm -rf portfolio_analysis_build
