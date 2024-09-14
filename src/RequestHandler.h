#ifndef REQUESTHANDLER_H
#define REQUSTHANDLER_H

#include "database.h"
#include "OperationTypes.h"
#include <vector>

class RequestHandler {
private:
  Database database;
  void parse_cmd_from_str(const std::string &cmdstr, std::vector<std::string> &cmd) const;
public:
  std::string handle_request(const std::string &cmd_str);
};

#endif // REQUESTHANDLER_H