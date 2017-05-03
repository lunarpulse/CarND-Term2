#ifndef PID_H
#define PID_H

class PID
{
public:
    /**
     * Constructor
     */
    PID(double target, double kp_init, double ki_init, double kd_init);

    /**
     * Destructor.
     */
    virtual ~PID();

    /**
     * Update the PID error variables given cross track error and return the estimated value.
     */
    double Update(double cte);

    /**
     * Change the target point for the PID
     * */
    void setTarget(double new_target) { set_point_ = new_target; }

    /**
     * Set the maximum value for the integral term. Helps prevent integral windup.
     * */
    void setMaxIntegral(double i_max) { max_integral_ = i_max; }

private:
    // Target for the PID to converge to.
    double set_point_;

    // error at t-1, used for derivative calculation
    double prev_error_;

    // previous timestamp, used for dt calculation
    std::time_t prev_timestamp_;

    // max integral term
    double max_integral_;

    // errors
    double p_error_;
    double i_error_;
    double d_error_;

    // coefficients
    double Kp_;
    double Ki_;
    double Kd_;
};

#endif /* PID_H */
