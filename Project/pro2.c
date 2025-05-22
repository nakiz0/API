<html>
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <title>Math Data Processing App</title>
  <script src="https://cdn.tailwindcss.com"></script>
  <link
    rel="stylesheet"
    href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.3/css/all.min.css"
  />
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
</head>
<body class="bg-gray-50 min-h-screen flex flex-col">
  <header class="bg-blue-600 text-white p-4 shadow-md">
    <h1 class="text-2xl font-bold text-center">Graphify</h1>
  </header>

  <main class="flex-grow container mx-auto p-4 max-w-4xl">
    <!-- Input Form -->
    <section class="bg-white rounded-lg shadow p-6 mb-8">
      <h2 class="text-xl font-semibold mb-4">Input New Data</h2>
      <form id="dataForm" class="space-y-4" novalidate>
        <div>
          <label for="personName" class="block font-medium mb-1">Person Name</label>
          <input
            type="text"
            id="personName"
            name="personName"
            required
            class="w-full border border-gray-300 rounded px-3 py-2 focus:outline-none focus:ring-2 focus:ring-blue-500"
            placeholder="Enter person's name"
            autocomplete="off"
          />
        </div>
        <div>
          <label for="scores" class="block font-medium mb-1">Scores (comma separated)</label>
          <input
            type="text"
            id="scores"
            name="scores"
            required
            pattern="^(\d+)(,\s*\d+)*$"
            class="w-full border border-gray-300 rounded px-3 py-2 focus:outline-none focus:ring-2 focus:ring-blue-500"
            placeholder="e.g. 78, 85, 90, 88"
            autocomplete="off"
          />
          <p class="text-sm text-gray-500 mt-1">Enter numeric scores separated by commas</p>
        </div>
        <div class="flex space-x-4">
          <button
            type="submit"
            class="bg-blue-600 text-white px-5 py-2 rounded hover:bg-blue-700 transition flex items-center justify-center"
          >
            <i class="fas fa-upload mr-2"></i> Submit Data
          </button>
          <button
            type="button"
            id="clearDataBtn"
            class="bg-red-600 text-white px-5 py-2 rounded hover:bg-red-700 transition flex items-center justify-center"
          >
            <i class="fas fa-trash-alt mr-2"></i> Clear Data
          </button>
        </div>
      </form>
      <p id="formMessage" class="mt-3 text-sm"></p>
    </section>

    <!-- Chart Display -->
    <section class="bg-white rounded-lg shadow p-6">
      <h2 class="text-xl font-semibold mb-4">Performance Chart</h2>
      <canvas id="performanceChart" class="w-full max-w-full h-64"></canvas>
    </section>
  </main>

  <script>
    // Since no backend is provided, use localStorage to simulate data storage

    // Helper to get stored data
    function getStoredData() {
      const stored = localStorage.getItem("graphifyData");
      if (!stored) return [];
      try {
        return JSON.parse(stored);
      } catch {
        return [];
      }
    }

    // Helper to save data
    function saveData(data) {
      localStorage.setItem("graphifyData", JSON.stringify(data));
    }

    // Calculate average score
    function calculateAverage(scores) {
      if (!scores.length) return 0;
      const sum = scores.reduce((a, b) => a + b, 0);
      return Math.round(sum / scores.length);
    }

    // Chart.js setup
    const ctx = document.getElementById("performanceChart").getContext("2d");
    let performanceChart = new Chart(ctx, {
      type: "bar",
      data: {
        labels: [],
        datasets: [
          {
            label: "Average Score",
            data: [],
            backgroundColor: "rgba(37, 99, 235, 0.7)",
            borderColor: "rgba(37, 99, 235, 1)",
            borderWidth: 1,
          },
        ],
      },
      options: {
        responsive: true,
        scales: {
          y: {
            beginAtZero: true,
            max: 100,
          },
        },
      },
    });

    // Update chart from localStorage data
    function updateChartFromStorage() {
      const data = getStoredData();
      performanceChart.data.labels = data.map((item) => item.name);
      performanceChart.data.datasets[0].data = data.map((item) => item.average);
      performanceChart.update();
    }

    updateChartFromStorage();

    // Handle form submission
    document.getElementById("dataForm").addEventListener("submit", (e) => {
      e.preventDefault();
      const formMessage = document.getElementById("formMessage");
      formMessage.textContent = "";
      formMessage.className = "";
      const nameInput = e.target.personName;
      const scoresInput = e.target.scores;
      const name = nameInput.value.trim();
      const scoresRaw = scoresInput.value.trim();

      if (!name) {
        formMessage.textContent = "Please enter a person's name.";
        formMessage.className = "text-red-600";
        nameInput.focus();
        return;
      }
      if (!scoresRaw) {
        formMessage.textContent = "Please enter scores.";
        formMessage.className = "text-red-600";
        scoresInput.focus();
        return;
      }
      const scoresPattern = /^(\d+)(,\s*\d+)*$/;
      if (!scoresPattern.test(scoresRaw)) {
        formMessage.textContent = "Scores must be numeric and comma separated.";
        formMessage.className = "text-red-600";
        scoresInput.focus();
        return;
      }

      const scores = scoresRaw.split(",").map((s) => parseInt(s.trim(), 10));
      if (scores.some(isNaN)) {
        formMessage.textContent = "Please enter valid numeric scores.";
        formMessage.className = "text-red-600";
        scoresInput.focus();
        return;
      }

      // Save data to localStorage
      const storedData = getStoredData();

      // Check if name already exists, update scores if so
      const existingIndex = storedData.findIndex((item) => item.name.toLowerCase() === name.toLowerCase());
      const average = calculateAverage(scores);
      if (existingIndex !== -1) {
        storedData[existingIndex].scores = scores;
        storedData[existingIndex].average = average;
      } else {
        storedData.push({ name, scores, average });
      }
      saveData(storedData);

      formMessage.textContent = "Data submitted successfully!";
      formMessage.className = "text-green-600";
      e.target.reset();
      updateChartFromStorage();
    });

    // Clear Data button handler
    document.getElementById("clearDataBtn").addEventListener("click", () => {
      const formMessage = document.getElementById("formMessage");
      formMessage.textContent = "";
      formMessage.className = "";
      localStorage.removeItem("graphifyData");
      performanceChart.data.labels = [];
      performanceChart.data.datasets[0].data = [];
      performanceChart.update();
      formMessage.textContent = "Data cleared successfully!";
      formMessage.className = "text-green-600";
    });
  </script>
</body>
</html>