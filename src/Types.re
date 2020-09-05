type todo = {
  __typename:string,
  id: string,
  text: string,
  completed: bool,
};

type user = {
  avatarUrl: option(string),
  fullName: string,
};

type workingGroup = {
  id: string,
  name: string,
};
type assignee = [ | `User(user) | `WorkingGroup(workingGroup)];

type status = [ | `Done | `Rejected | `OnHold | `Progress];

type ticket = {
  id: string,
  status,
  subject: string,
  assignee: option(assignee),
  lastUpdated: option(Js.Date.t),
};

type siteStatistics = {
  weeklySales: float,
  weeklyOrders: int,
  currentVisitorsOnline: int,
};
