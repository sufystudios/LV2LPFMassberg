BUNDLE = amp.lv2
INSTALL_DIR = /usr/local/lib/lv2


$(BUNDLE): manifest.ttl amp.so amp.ttl
	rm -rf $(BUNDLE)
	mkdir $(BUNDLE)
	cp manifest.ttl amp.ttl amp.so  $(BUNDLE)

amp.so: amp.cpp Filter.o MoogLadderFilter.o VAOnePoleFilter.o
	g++ -shared -fpermissive -fPIC amp.cpp -o amp.so Filter.o MoogLadderFilter.o VAOnePoleFilter.o -lbsd -LV2

Filter.o: Filter.cpp Filter.h
	g++ -fPIC -c Filter.cpp

Oscillator.o: Oscillator.cpp Oscillator.h
	g++ -fPIC -c Oscillator.cpp

LFO.o: LFO.cpp LFO.h
	g++ -fPIC -DPIC -c LFO.cpp

EnvelopeGenerator.o: EnvelopeGenerator.cpp EnvelopeGenerator.h
	g++ -fPIC -DPIC -c EnvelopeGenerator.cpp

VAOnePoleFilter.o: VAOnePoleFilter.cpp VAOnePoleFilter.h
	g++ -fPIC -DPIC -c VAOnePoleFilter.cpp

MoogLadderFilter.o: MoogLadderFilter.cpp MoogLadderFilter.h
	g++ -fPIC -DPIC -c MoogLadderFilter.cpp

DCA.o: DCA.cpp DCA.h
	g++ -fPIC -DPIC -c DCA.cpp

ModulationMatrix.o: ModulationMatrix.cpp ModulationMatrix.h
	g++ -fPIC -DPIC -c ModulationMatrix.cpp

QBLimitedOscillator.o: QBLimitedOscillator.cpp QBLimitedOscillator.h
	g++ -fPIC -DPIC -c QBLimitedOscillator.cpp

manifest.ttl: manifest.ttl.in manifest.ttl
	rm manifest.ttl
	cp manifest.ttl.in manifest.ttl

manifest.ttl: manifest.ttl.in
		cp manifest.ttl.in manifest.ttl

install: $(BUNDLE)
	mkdir -p $(INSTALL_DIR)
	rm -rf $(INSTALL_DIR)/$(BUNDLE)
	cp -R $(BUNDLE) $(INSTALL_DIR)

clean:
	rm -rf $(BUNDLE) amp.so amp.o Filter.o Oscillator.o QBLimitedOscillator.o EnvelopeGenerator.o MoogLadderFilter.o DCA.o ModulationMatrix.o VAOnePoleFilter.o LFO.o
