"""Measure ordered user conversion funnels from CSV events."""
import argparse
import collections
import csv
import datetime as dt
import json


def parse_time(value):
    parsed=dt.datetime.fromisoformat(value.replace("Z","+00:00"))
    if parsed.tzinfo is None:
        raise ValueError("event timestamps require timezone")
    return parsed.astimezone(dt.timezone.utc)


def analyze(events,steps):
    if not isinstance(steps,list) or not steps or len(set(steps))!=len(steps):
        raise ValueError("steps must be a nonempty unique list")
    by_user=collections.defaultdict(list)
    for event in events:
        user=event["user_id"].strip()
        name=event["event"].strip()
        if not user or not name:
            raise ValueError("user_id and event are required")
        by_user[user].append((parse_time(event["timestamp"]),name))
    reached=[0]*len(steps)
    for timeline in by_user.values():
        stage=0
        for _,name in sorted(timeline):
            if stage<len(steps) and name==steps[stage]:
                reached[stage]+=1
                stage+=1
    return {"users":len(by_user),"steps":[{"event":name,"users":reached[index],
             "conversion_from_first":round(reached[index]/reached[0],4) if reached[0] else None,
             "conversion_from_previous":round(reached[index]/reached[index-1],4) if index and reached[index-1] else (1.0 if index==0 and reached[0] else None)}
            for index,name in enumerate(steps)]}


def run(path,steps):
    with open(path,newline="",encoding="utf-8") as source:
        return analyze(csv.DictReader(source),steps)


def main():
    parser=argparse.ArgumentParser(description="Analyze ordered event conversion funnels")
    parser.add_argument("csv_file")
    parser.add_argument("--steps",nargs="+",required=True)
    args=parser.parse_args()
    print(json.dumps(run(args.csv_file,args.steps),indent=2))


if __name__=="__main__":
    main()
