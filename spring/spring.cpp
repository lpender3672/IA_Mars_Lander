#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {

  // declare variables
  double m, k, x, v, t_max, dt, t, a, x_prev, x_current;
  vector<double> t_list, x_list, v_list;

  // mass, spring constant, initial position and velocity
  m = 1;
  k = 1;
  x = 0;
  v = 1;

  // simulation time and timestep
  t_max = 100;
  dt = 0.1;

  // Euler integration
  for (t = 0; t <= t_max; t = t + dt) {

    // append current state to trajectories
    t_list.push_back(t);
    x_list.push_back(x);
    v_list.push_back(v);

    if (t == 0) {
        x_prev = -(v * dt - x);
    }
    else {
		x_prev = x_list[t_list.size() - 2];
    }

    // calculate new position and velocity
    a = -k * x / m;
    x_current = x;
    x = 2 * x - x_prev + dt * dt * a;
    v = (x - x_current) / dt;

  }

  // Write the trajectories to file
  ofstream fout;
  fout.open("trajectories.txt");
  if (fout) { // file opened successfully
    for (int i = 0; i < t_list.size(); i = i + 1) {
      fout << t_list[i] << ' ' << x_list[i] << ' ' << v_list[i] << endl;
    }
    fout.close(); // added saftey
  } else { // file did not open successfully
    cout << "Could not open trajectory file for writing" << endl;
  }
  system("display_data.py");

  return 0;
}
