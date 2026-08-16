# OME
An Order Matching Engine written in C++. 

## OrderBook Design: 
- Two ordered maps
    - One that is monotonically increasing for asks (unfulfilled sell orders) 
    - One that is monotonically decreasing for bids (unfulfilled buy orders) 

- Each map is from price -> DLL 
    - The head of the DLL represents the earliest order 
    - We always start from the head when processing an order 
    - We always add to the tail when a new unfulfilled order comes in  

- We have an unordered map from orderId -> node 
    - On cancels, we remove the node in-place 
    - No mapping for orderId means the order is cancelled/does not exist 

## Testing Strategy 
- Used GoogleTest
    - Integrated into the CMake build
    - Set up fixtures (classes) to set up testing environments for each test 

## Build Strategy 
- Different executables for main entry point and tests 
- Create a shared target for the core logic that main and tests share  
- Pull in GoogleTest via FetchContent
