class Filter{
private:
	float* out;
	float* in;
	float alpha = 0.05;
public:
	Filter(float* _out, float* _in){
		out = _out;
		in = _in;
	}
	void update(){
		*out = alpha* (*in) + (1-alpha) * (*out);
	}
};