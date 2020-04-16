#ifndef __EDGEDETECTOR_H__
#define __EDGEDETECTOR_H__

enum class EdgeType {
    kFalling = FALLING,
    kNoEdge = 0,
    kRising = RISING
};

template<typename T> 
class EdgeDetector {
    public:
        /*! Return the current value. */
        T& getValue() {
            return value;
        }

        /*! Set a new value withut edge detection. */
        void setValue(T newValue) {
            this->value = newValue;
        }

        /*! Set a new value and return whether the new value is smaller, equal or larger to the old value. */
        EdgeType setValueDetectEdge(T newValue) {           
            EdgeType edge = EdgeType::kNoEdge;
            if (this->value < newValue) {
                edge = EdgeType::kRising;
            } else if (newValue < this->value) {
                edge = EdgeType::kFalling;
            }
            value = newValue;
            return edge;
        }
        
    private:
        T value;
};

using BoolEdgeDetector = EdgeDetector<bool>;

#endif //  __EDGEDETECTOR_H__
