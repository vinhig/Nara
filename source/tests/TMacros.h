#define ASSERT_THROW(condition, msg)                                      \
  {                                                                       \
    if (!(condition)) {                                                   \
      throw std::runtime_error(std::string(__FILE__) + std::string(":") + \
                               std::to_string(__LINE__) +                 \
                               std::string(" in ") +                      \
                               std::string(__PRETTY_FUNCTION__));         \
    }                                                                     \
  }

#define ASSERT_EQUAL(x, y, msg)                                             \
  {                                                                         \
    if ((x) != (y)) {                                                       \
      throw std::runtime_error(                                             \
          std::string(__FILE__) + std::string(":") +                        \
          std::to_string(__LINE__) + std::string(" in ") +                  \
          std::string(__PRETTY_FUNCTION__) + std::string(": ") +            \
          std::to_string((x)) + std::string(" != ") + std::to_string((y)) + \
          std::string("\n") + msg);                                         \
    }                                                                       \
  }