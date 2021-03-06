//File: nonlin.hh
//Author: Yuxin Wu <ppwwyyxxc@gmail.com>

#pragma once
#include "layer.hh"

namespace hadnn {

class ReLU : public Layer {
	public:
		ReLU(Layer* top): Layer(top) {
			ndim_ = top->out_dim();
			setup();
		}

		void setup() {
			auto& input = tops_[0]->get_output();
			switch (ndim_) {
				case 1:
					output_(x) = max(input(x), 0);
					break;
				case 2:
					output_(x, y) = max(input(x, y), 0);
					break;
				case 3:
					output_(x, y, z) = max(input(x, y, z), 0);
					break;
				case 4:
					output_(x, y, z, w) = max(input(x, y, z, w), 0);
					break;
				default:
					error_exit("Unsupported Dimension");
			}
		}

		void default_sched() override {
			if (ndim_ == 4) {
				Halide::Var par;
				output_.fuse(w, z, par).parallel(par);
			}
			// TODO
		}

		int out_dim() const override { return ndim_; }

		ShapeExpr out_shape() const override { return tops_[0]->out_shape(); }

		Halide::Var x{"relux"}, y{"reluy"}, z{"reluz"}, w{"reluw"};
	protected:
		int ndim_;

};

}	// namespace hadnn
