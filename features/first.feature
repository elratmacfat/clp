# language: en
Feature: NaturalNumberParameterType
  Check if passed string is a natural number.

  Scenario Outline: Blah
    Given I have passed <value> as parameter
    Then <result> should be returned

  Examples:
    | value | result |
    | 10    | 1      |
    | 0     | 1      |
    | -1    | 0      |
    | a     | 0      |
