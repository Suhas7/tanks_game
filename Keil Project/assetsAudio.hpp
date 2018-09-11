//ASSETS
class soundByte{
	public:
	const unsigned char* samples;
	int samplesize;
		soundByte(const unsigned char* samplesIn, int sampleSize){
			samples=samplesIn;samplesize=sampleSize;
		}
};
