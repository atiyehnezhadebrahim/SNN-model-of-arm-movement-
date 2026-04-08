import opensim as osim
print(dir(osim))
import pandas as pd
import numpy as np

import matplotlib.pyplot as plt

# Load the arm model
model = osim.Model('/home/atiyeh/Biophysical-model-of-CPG-main/arm_model.osim')

# List all actuators in the model
for i in range(model.getActuators().getSize()):
    print(model.getActuators().get(i).getName())

# Enable the visualizer
model.setUseVisualizer(True)

# Initialize the system
state = model.initSystem()

# Load excitation data
data = pd.read_csv(
    '/home/atiyeh/Biophysical-model-of-CPG-main/saved_simulations/2025-05-13-11-28-36/arm_excitation.sto',
    sep=r'\s+',
    skiprows=6
)

time_array = data["time"].to_numpy()
biceps_array = data["BIClong"].to_numpy()
triceps_array = data["TRIlong"].to_numpy()

# Create a PrescribedController
controller = osim.PrescribedController()
biceps_func = osim.PiecewiseLinearFunction()
triceps_func = osim.PiecewiseLinearFunction()

# Add excitation data to the functions
for t, b, tr in zip(time_array, biceps_array, triceps_array):
    biceps_func.addPoint(t, b)
    triceps_func.addPoint(t, tr)

# Add muscles to the controller
biceps = model.getMuscles().get('BIClong')
triceps = model.getMuscles().get('TRIlong')
controller.addActuator(biceps)
controller.addActuator(triceps)
controller.prescribeControlForActuator('BIClong', biceps_func)
controller.prescribeControlForActuator('TRIlong', triceps_func)

# Add the controller to the model
model.addController(controller)

# Re-initialize the system after adding the controller
state = model.initSystem()

# Create the manager with the model
manager = osim.Manager(model)

# Define initial and final times
initial_time = time_array[0]
final_time = time_array[-1]


#print(dir(manager))

# Set the initial time
state.setTime(initial_time)

# Create the manager with the model and state
manager = osim.Manager(model, state)

# Integrate the model to the final time
state = manager.integrate(final_time)

states_table = manager.getStatesTable()


# Extract column labels and data
column_labels = states_table.getColumnLabels()
time = states_table.getIndependentColumn()
data = np.array([states_table.getDependentColumn(label).to_numpy() for label in column_labels]).T

# Create DataFrame
df = pd.DataFrame(data, columns=column_labels)
df.insert(0, 'time', time)
print(df.columns.tolist())
for col in df.columns:
    print(col)

df.columns = df.columns.str.replace('/', '_')
print(df.columns.tolist())




#Plotting 'BIClong.activation' over time
plt.figure()

# Plot BIC long head
plt.plot(df['time'], df['_forceset_BIClong_activation'], label='BIClong')

# Plot TRI long head
plt.plot(df['time'], df['_forceset_TRIlong_activation'], label='TRIlong')

# Labels and grid
plt.xlabel('Time (s)')
plt.ylabel('Muscle Activation')
plt.title('Muscle Activation Over Time')
plt.grid(True)

# Plot elbow joint position and speed
plt.figure()

# Elbow angle (position)
plt.plot(df['time'], df['_jointset_elbow_r_elbow_flexion_value'], label='Elbow Angle (rad)')

# Elbow speed
plt.plot(df['time'], df['_jointset_elbow_r_elbow_flexion_speed'], label='Elbow Angular Speed (rad/s)')

# Labels and grid
plt.xlabel('Time (s)')
plt.ylabel('Joint Metrics')
plt.title('Elbow Joint Angle and Speed Over Time')





# Show plot
plt.show()
