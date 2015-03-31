

#ifndef LINES_H_
#define LINES_H_

    class Lines{
        public:
	        /**
	         * Constructor.
	         *
	         * @param argc Number of command line arguments.
	         * @param argv Command line arguments.
	         */
	        Lines(double x, double y);

	        double getXPos();
	        double getYPos();
	        void setXPos(double x);
	        double getCritical();

        private:
            double xPos;
			double yPos;
			double critical;


    };


#endif /*LINES_H_*/