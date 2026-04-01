import os
import sys
import subprocess
import json
import argparse
import time

SCRAMBLES = [
    "D L B2 R2 B' R2 U2 L2 B2 U2 B D2 L2 R' U",
    "F2 U' R2 D' B2 D2 F2 D B2 D F2 R F' U R'",
    "B2 U B2 R2 U2 L2 U L2 U B2 D B D F R'",
    "R2 F2 R2 U R2 D' F2 U B2 U L2 F D' B2",
    "L2 D2 F2 U' L2 D2 B2 D2 F2 U F' U' R B",
    "U L2 U' B2 R2 D' L2 F2 U' L2 U' B U R'",
    "D' R2 U F2 R2 D R2 U' F2 D F2 L R' D'",
    "F2 U2 R2 B' R2 F U2 B R2 B2 U B' L R'",
    "U' B2 D B2 F2 D L2 D2 F2 U L2 F L2 R2",
    "R2 D2 F2 D B2 U' R2 D' R2 B2 D B' L2 B"
]

def get_root_dir():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    return os.path.dirname(script_dir)

def build_solver():
    root_dir = get_root_dir()
    print("Building solver in " + root_dir + "...")
    result = subprocess.run(["make"], cwd=root_dir, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    if result.returncode != 0:
        print("Build failed!")
        print(result.stderr)
        sys.exit(1)
    
    solver_path = os.path.join(root_dir, "solver")
    if not os.path.exists(solver_path):
        print("Executable solver not found after build!")
        sys.exit(1)
    print("Build successful.")

def run_scramble(scramble):
    root_dir = get_root_dir()
    solver_path = os.path.join(root_dir, "solver")
    result = subprocess.run([solver_path, scramble], cwd=root_dir, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"Solver crashed on scramble: {scramble}")
        print(result.stderr)
        return None

    # Parse output
    output = result.stdout
    metrics = {
        "states": 0,
        "time": 0.0,
        "speed": 0.0,
        "solved": False
    }

    for line in output.split('\n'):
        if "SOLUTION FOUND" in line:
            metrics["solved"] = True
        elif "Total states:" in line:
            # e.g., "Total states: 12,345"
            parts = line.split(':')
            val_str = parts[1].replace(',', '').strip()
            if val_str:
                metrics["states"] = int(val_str)
        elif "Search time:" in line:
            # e.g., "Search time:  0.123 seconds"
            parts = line.split(':')
            val_str = parts[1].replace('seconds', '').strip()
            if val_str:
                metrics["time"] = float(val_str)
        elif "Speed:" in line:
            # e.g., "Speed:        100,000 states/sec"
            parts = line.split(':')
            val_str = parts[1].replace('states/sec', '').replace(',', '').strip()
            if val_str:
                metrics["speed"] = float(val_str)

    return metrics

def print_diff(baseline_val, current_val, metric_name, inverse=False):
    if baseline_val == 0:
        return f"{current_val:>10.2f} (N/A)"
    
    diff = current_val - baseline_val
    pct = (diff / baseline_val) * 100
    
    # inverse=False means higher is better (e.g. speed)
    # inverse=True means lower is better (e.g. time)
    if pct == 0:
        color = "\033[90m" # gray
        sign = ""
    elif (pct > 0 and not inverse) or (pct < 0 and inverse):
        color = "\033[92m" # green
        sign = "+" if pct > 0 else ""
    else:
        color = "\033[91m" # red
        sign = "+" if pct > 0 else ""

    return f"{current_val:>10.2f} ({color}{sign}{pct:.1f}%\033[0m)"

def main():
    parser = argparse.ArgumentParser(description="Rubik's Cube Solver Benchmark Suite")
    parser.add_argument("--save-baseline", nargs="?", const="baseline", default=None, help="Run benchmark and save as a named baseline (default: 'baseline')")
    args = parser.parse_args()

    build_solver()

    results = []
    print(f"\nRunning {len(SCRAMBLES)} benchmark scrambles...")
    
    for i, scramble in enumerate(SCRAMBLES):
        print(f"[{i+1}/{len(SCRAMBLES)}] Scramble: {scramble}")
        metrics = run_scramble(scramble)
        if metrics:
            results.append({
                "scramble": scramble,
                "metrics": metrics
            })
            print(f"   -> {metrics['time']:.3f}s, {metrics['speed']:.0f} states/sec")
        else:
            print("   -> FAILED")

    # Aggregate
    total_time = sum(r["metrics"]["time"] for r in results)
    total_states = sum(r["metrics"]["states"] for r in results)
    avg_speed = total_states / total_time if total_time > 0 else 0

    run_summary = {
        "total_time": total_time,
        "total_states": total_states,
        "avg_speed": avg_speed,
        "runs": results
    }

    print("\n" + "="*50)
    print("BENCHMARK RESULTS")
    print("="*50)
    print(f"Total Time:   {total_time:.3f} seconds")
    print(f"Total States: {total_states:,}")
    print(f"Average Speed: {avg_speed:,.0f} states/second")
    run_summary["avg_speed"] = avg_speed

    baseline_path = os.path.join(get_root_dir(), "benchmark", "baseline.json")

    if args.save_baseline is not None:
        name = args.save_baseline
        baselines = {}
        if os.path.exists(baseline_path):
            with open(baseline_path, "r") as f:
                try:
                    baselines = json.load(f)
                except json.JSONDecodeError:
                    pass
        
        baselines[name] = run_summary
        with open(baseline_path, "w") as f:
            json.dump(baselines, f, indent=4)
        print(f"\nBaseline '{name}' saved to {baseline_path}")
    else:
        if os.path.exists(baseline_path):
            with open(baseline_path, "r") as f:
                baselines = json.load(f)
            
            for name, baseline in baselines.items():
                print(f"\nCOMPARISON WITH BASELINE: {name}")
                print("-" * 50)
                print(f"Metric         | Baseline   | Current          | Change")
                print("-" * 50)
                print(f"Total Time (s) | {baseline['total_time']:<10.3f} | {print_diff(baseline['total_time'], total_time, 'time', inverse=True)}")
                print(f"Net Speed      | {baseline['avg_speed']:<10.0f} | {print_diff(baseline['avg_speed'], avg_speed, 'speed')}")
                print(f"Total States   | {baseline['total_states']:<10,} | {print_diff(baseline['total_states'], total_states, 'states', inverse=True)}")
                print("-" * 50)
        else:
            print("\nNo baseline.json found. Run with --save-baseline to set a baseline.")

if __name__ == "__main__":
    main()
