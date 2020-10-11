//
// Created by norbe on 17/09/2020.
//


#include <stdbool.h>
#include <stdio.h>
#include <unity.h>
#include "../Core/YPVS/Ypvs.h"
#include "../lib/CircularBuffer/circularBuffer.h"

ypvsMapStep TestSteps[MAX_STEPS] = {
        {3000, 0},
        {3300, 10},
        {3600, 20},
        {3900, 30},
        {4200, 40},
        {4500, 50},
        {4800, 60},
        {5100, 70},
        {5400, 80},
        {5700, 90},
        {6000, 100},
};


void getCurrentStepTest(){
  ypvsMap testMap = *ypvsMap_Init(&testMap,TestSteps,3000,12000);
  TEST_ASSERT_EQUAL_HEX32(50, getCurrentStep(&testMap,4621).ypvsOpenPercentage);
}

void testGetCurrentStepWhenRPMAreGoingUp(){
  char testMessage[40];

  ypvsMap testMap = *ypvsMap_Init(&testMap,TestSteps,3000,12000);
  for(int i = 0; i < MAX_STEPS; i++){
    uint16_t expectedPercentage = testMap.steps[i].ypvsOpenPercentage;
    uint16_t actualRPM = testMap.steps[i].rpm;
    sprintf(testMessage,"Current rpm: %u Valve open on %u%%", actualRPM, expectedPercentage);
    TEST_MESSAGE(testMessage);
    TEST_ASSERT_EQUAL_HEX16(expectedPercentage, getCurrentStep(&testMap,actualRPM).ypvsOpenPercentage);
  }
}


void testGetCurrentStemWhenRPMAreGoingDown(){
  char testMessage[40];

  ypvsMap testMap = *ypvsMap_Init(&testMap,TestSteps,3000,12000);
  for(int i = MAX_STEPS - 1; i >= 0; i--){
    uint16_t expectedPercentage = testMap.steps[i].ypvsOpenPercentage;
    uint16_t actualRPM = testMap.steps[i].rpm - 90;
    sprintf(testMessage,"Current rpm: %u Valve open on %u%%", actualRPM, expectedPercentage);
    TEST_MESSAGE(testMessage);
    TEST_ASSERT_EQUAL_INT(expectedPercentage, getCurrentStep(&testMap,actualRPM).ypvsOpenPercentage);
  }
}


void testAverageRPMValue() {
  uint16_t testArrayForCircularBuffer[25] = {
          4000, 4150, 4050, 4200, 4231,
          4312, 6234, 3233, 3942, 4011,
          4421, 4450, 4421, 4490, 4520,
          4511, 4566, 4560, 4420, 4000,
          4611, 4445, 4535, 4569, 4642,
  };
  const uint16_t expected = 4380;

  CircularBuffer_t circularBuffer;
  CircularBuffer_Init(&circularBuffer, testArrayForCircularBuffer, 25);

  TEST_ASSERT_EQUAL_INT(expected, CircularBuffer_GetAverageValue(&circularBuffer));
}





int main(){
  UNITY_BEGIN();
  RUN_TEST(testGetCurrentStepWhenRPMAreGoingUp);
  RUN_TEST(testGetCurrentStemWhenRPMAreGoingDown);
  RUN_TEST(testAverageRPMValue);
  return UnityEnd();
}

