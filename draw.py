import pandas as pd
import matplotlib.pyplot as plt

puntos = pd.read_csv("puntos.csv")
triangulos = pd.read_csv("triangulos.csv")
voronoi = pd.read_csv("voronoi_aristas.csv")

fig, ax = plt.subplots(figsize=(8, 6))


for row in triangulos.itertuples():
    ax.plot([row.x1, row.x2], [row.y1, row.y2], color="lightgray", linewidth=0.8)

for row in voronoi.itertuples():
    ax.plot([row.x1, row.x2], [row.y1, row.y2], color="steelblue", linewidth=1.2)


ax.scatter(puntos['x'], puntos['y'], color="black", s=15, zorder=5)

ax.set_aspect('equal') # Equivalente a coord_fixed()
ax.set_title("Triangulación de Delaunay y Diagrama de Voronoi")
ax.set_xlabel("X")
ax.set_ylabel("Y")

ax.spines['top'].set_visible(False)
ax.spines['right'].set_visible(False)
ax.grid(True, linestyle='--', alpha=0.5)

plt.savefig("voronoi_delaunay.png", dpi=300, bbox_inches='tight')
plt.show()
